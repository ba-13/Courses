#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static char *ERR_MSG = "Unable to execute";
static unsigned long total_size = 0;
static char VERBOSE = 0;

void err_exit()
{
	fprintf(stderr, "%s: %s", ERR_MSG, strerror(errno));
	exit(-1);
}

void print_indents(int indent)
{
	for (int count = 0; count < indent; count++)
		printf("\t");
}

void generate_directory_path(char *base_path, char *subdir, char *buf)
{
	strcpy(buf, base_path);
	strcat(buf, "/");
	strcat(buf, subdir); // changes buf in-place
}

void traverse(char *fn, int indent)
{
	DIR *dir;
	struct dirent *entry;
	char path[4096];
	struct stat info;

	int statusReturn = stat(fn, &info);
	if (statusReturn != 0)
		err_exit();

	if (VERBOSE)
	{
		printf("%ld\t", info.st_size);
		print_indents(indent);
		printf("%s\n", fn);
	}

	total_size += info.st_size;

	if (!S_ISDIR(info.st_mode))
		return;

	if ((dir = opendir(fn)) == NULL)
		err_exit();
	else
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name[0] == '.')
				continue;
			generate_directory_path(fn, entry->d_name, path);
			traverse(path, indent + 1);
		}
		closedir(dir);
	}
}

void spawn_children(char *path)
{
	DIR *dir;
	struct stat info;
	struct dirent *dir_entry;
	char buf[4096];
	int indent = 0;

	int status_return = stat(path, &info);
	total_size += info.st_size; // add root's size
	if (status_return != 0 || !S_ISDIR(info.st_mode))
		err_exit();

	if (VERBOSE)
	{
		printf("%ld\t", info.st_size);
		print_indents(indent);
		printf("%s\n", path);
	}

	dir = opendir(path); // fd 3 allocated
	if (dir == NULL)
		err_exit();
	int num_dirs = 0;
	while ((dir_entry = readdir(dir)) != NULL)
	{
		// won't count hidden files or current/parent directory
		if (dir_entry->d_name[0] == '.')
			continue;

		generate_directory_path(path, dir_entry->d_name, buf);

		// get info about this file, of symlink as well
		status_return = lstat(buf, &info);

		if (status_return != 0)
			err_exit();

		switch (info.st_mode & 0170000)
		{
		case 0120000: // LNK
			traverse(buf, 1);
			break;
		case 0100000: // FIL
			total_size += info.st_size;
			break;
		case 0040000: // DIR
			num_dirs++;
			// create pipe to communicate
			int fd[2];
			int pipe_return = pipe(fd);

			// fork to subprocess
			pid_t pid = fork();
			if (pid < 0 || pipe_return == -1)
				err_exit();
			if (!pid)
			{
				// child
				close(fd[0]); // close read end
				total_size = 0;
				traverse(buf, 1); // fill-up total_size for this process
				write(fd[1], &total_size, sizeof(total_size));
				close(fd[1]);
				exit(0);
			}
			else
			{
				// parent
				close(fd[1]); // close write end
			}
			break;
		}
	}
	// relies on fact that fd from 4 to above are connected to pipes allocated in ISDIR
	for (int fd = 4; fd < 4 + num_dirs; fd++)
	{
		unsigned long child_output_bytes;
		read(fd, &child_output_bytes, sizeof(child_output_bytes));
		total_size += child_output_bytes;
		close(fd);
	}
	closedir(dir);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "%s\n", ERR_MSG);
		return -1;
	}
	spawn_children(argv[1]);
	printf("%ld\n", total_size);
	return 0;
}
