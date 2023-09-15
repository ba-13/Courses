#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char VERBOSE = 0;

long function(long val)
{
	return val * val;
}

int main(int argc, char **argv)
{
	long val;
	if (VERBOSE)
	{
		printf("---\n");
		for (int i = 0; i < argc; i++)
			printf("%s\n", argv[i]);
	}
	val = strtol(argv[argc - 1], NULL, 10);
	val = function(val);

	if (argc == 2)
	{
		printf("%ld\n", val);
		return 0;
	}

	char buf[100];
	sprintf(buf, "%ld", val);
	argv[argc - 1] = buf;
	argv = &(argv[1]);
	execv(argv[0], argv);
	fprintf(stderr, "Unable to execute %s\n", argv[0]);
	return 1;
}