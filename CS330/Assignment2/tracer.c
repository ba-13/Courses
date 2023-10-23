#include <context.h>
#include <memory.h>
#include <lib.h>
#include <entry.h>
#include <file.h>
#include <tracer.h>

static const int verbose = 0;
/**
 * Start of Trace buffer functionality
 * */

static u32 check_permissions(u32 allowed, u32 requested)
{
	return (allowed & requested) == requested;
}

static char is_within_range(unsigned long start_range, unsigned long start_next_range, unsigned long start, u32 duration)
{
	return (start_range <= start && start + duration - 1 < start_next_range);
}

int is_valid_mem_range(unsigned long buff, u32 count, int access_bit)
{
	struct exec_context *current = get_current_ctx();
	struct mm_segment *mms = current->mms;
	struct vm_area *vm_area = current->vm_area;
	char is_valid = 0;
	int requested_access = access_bit & 3;
	for (int i = 0; i < 4; i++)
	{
		unsigned long end_mms_ptr = mms[i].next_free;
		if (i == 3)
			end_mms_ptr = mms[i].end;
		if (is_within_range(mms[i].start, end_mms_ptr, buff, count))
		{
			int allowed_access = mms[i].access_flags & 3;
			if (check_permissions(allowed_access, requested_access))
				return 0;
			else
				return -EACCES;
		}
	}
	while (vm_area)
	{
		if (is_within_range(vm_area->vm_start, vm_area->vm_end, buff, count))
		{
			int allowed_access = vm_area->access_flags & 3;
			if (check_permissions(allowed_access, requested_access))
				return 0;
			else
				return -EACCES;
		}
		vm_area = vm_area->vm_next;
	}
	return -EBADMEM;
}

long trace_buffer_close(struct file *filep)
{
	if (filep == NULL || filep->trace_buffer == NULL)
		return -EINVAL;
	os_page_free(USER_REG, filep->trace_buffer->base_pos);
	os_free(filep->trace_buffer, sizeof(struct trace_buffer_info));
	os_free(filep->fops, sizeof(struct fileops));
	os_free(filep, sizeof(struct file));
}

static int raw_trace_buffer_read(struct file *filep, char *buff, u32 count)
{
	char *bp = filep->trace_buffer->base_pos;
	char *wp = filep->trace_buffer->write_pos;
	char *rp = filep->trace_buffer->read_pos;
	int gap_wp_bp = wp - bp;
	int gap_rp_bp = rp - bp;
	int cnt = 0;
	if (rp >= wp && filep->trace_buffer->has_data)
	{
		int upper_bound = TRACE_BUFFER_MAX_SIZE - gap_rp_bp - 1;
		for (int i = 0; i <= upper_bound; i++)
		{
			buff[cnt] = *rp++;
			cnt++;
			count--;
			if (count == 0)
				break;
		}
		if (cnt == upper_bound + 1)
			rp = bp;
	}
	if (wp > rp)
	{
		for (int i = 0; i < gap_wp_bp; i++)
		{
			buff[cnt] = *rp++;
			cnt++;
			count--;
			if (count == 0)
				break;
		}
	}
	filep->trace_buffer->read_pos = rp;
	if (rp == wp)
		filep->trace_buffer->has_data = 0;
	if (verbose > 2)
		printk("[OS] [tb->buff] Read %dB from tb\n", cnt);
	return cnt;
}

int trace_buffer_read(struct file *filep, char *buff, u32 count)
{
	if (filep == NULL || filep->trace_buffer == NULL)
		return -EINVAL;
	int buf_validity = is_valid_mem_range((unsigned long)buff, count, 2);
	if (buf_validity == -EACCES || buf_validity == -EBADMEM)
		return -EBADMEM;
	if (!check_permissions(filep->mode & 3, O_READ))
		return -EINVAL;

	return raw_trace_buffer_read(filep, buff, count);
}

static int raw_trace_buffer_write(struct file *filep, char *buff, u32 count)
{
	char *bp = filep->trace_buffer->base_pos;
	char *wp = filep->trace_buffer->write_pos;
	char *rp = filep->trace_buffer->read_pos;
	int gap_wp_bp = wp - bp;
	int gap_rp_bp = rp - bp;
	int cnt = 0;
	if (wp >= rp)
	{
		int upper_bound = TRACE_BUFFER_MAX_SIZE - gap_wp_bp - 1;
		for (int i = 0; i <= upper_bound; i++)
		{
			*wp++ = buff[cnt];
			cnt++;
			count--;
			if (count == 0)
				break;
		}
		if (cnt == upper_bound + 1)
			wp = bp;
	}
	if (rp > wp)
	{
		for (int i = 0; i < gap_rp_bp; i++)
		{
			*wp++ = buff[cnt];
			cnt++;
			count--;
			if (count == 0)
				break;
		}
	}
	filep->trace_buffer->write_pos = wp;
	if (cnt > 0)
		filep->trace_buffer->has_data = 1;
	if (verbose > 2)
		printk("[OS] [buff->tb] Written %dB to tb\n", cnt);
	return cnt;
}

int trace_buffer_write(struct file *filep, char *buff, u32 count)
{
	if (filep == NULL || filep->trace_buffer == NULL)
		return -EINVAL;
	int buf_validity = is_valid_mem_range((unsigned long)buff, count, 1);
	if (buf_validity == -EACCES || buf_validity == -EBADMEM)
		return -EBADMEM;
	if (!check_permissions(filep->mode & 3, O_WRITE))
		return -EINVAL;

	return raw_trace_buffer_write(filep, buff, count);
}

int sys_create_trace_buffer(struct exec_context *current, int mode)
{
	struct file **curr_fds = current->files;
	int free_fd = -1;
	for (int i = 0; i < MAX_OPEN_FILES; i++)
	{
		if (*(curr_fds + i) == NULL)
		{
			free_fd = i;
			break;
		}
	}
	if (free_fd == -1)
		return -EINVAL;

	struct file *tb = (struct file *)os_alloc(sizeof(struct file));
	if (tb == NULL)
		return -EINVAL;
	current->files[free_fd] = tb;
	tb->mode = mode;
	tb->ref_count = 1; // the calling function
	tb->fops = (struct fileops *)os_alloc(sizeof(struct fileops));
	if (tb->fops == NULL)
		return -EINVAL;
	tb->fops->read = trace_buffer_read;
	tb->fops->write = trace_buffer_write;
	tb->fops->lseek = NULL;
	tb->fops->close = trace_buffer_close;
	tb->trace_buffer = (struct trace_buffer_info *)os_alloc(sizeof(struct trace_buffer_info));
	if (tb->trace_buffer == NULL)
		return -EINVAL;
	tb->trace_buffer->base_pos = (char *)os_page_alloc(USER_REG);
	if (tb->trace_buffer->base_pos == NULL)
		return -EINVAL;
	tb->trace_buffer->read_pos = tb->trace_buffer->base_pos;
	tb->trace_buffer->write_pos = tb->trace_buffer->base_pos;
	tb->trace_buffer->has_data = 0;
	return free_fd;
}

/**
 * Start of strace functionality
 * */

static int syscall_args[70] = {
	-1,
	1, // exit 1
	0, // getpid 2
	-1,
	2, // expand 4
	-1,
	-1,
	1, // sleep 7
	2, // signal 8
	2, // clone 9
	0, // fork 10
	0, // stats 11
	1, // configure 12
	0, // phys info 13
	1, // dump ptt 14
	0, // cfork 15
	4, // mmap 16
	2, // munmap 17
	3, // mprotect 18
	1, // pmap 19
	0, // vfork 20
	0, // get userp 21
	0, // get cowf 22
	2, // open 23
	3, // read 24
	3, // write 25
	-1,
	1, // dup 27
	2, // dup2 28
	1, // close 29
	3, // lseek 30
	-1,
	-1,
	-1,
	-1,
	4, // ftrace 35
	1, // tracebuf 36
	2, // start strace 37
	0, // end strace 38
	3, // read strace 39
	2, // strace 40
	3, // read ftrace 41
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1, // 50
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1, // 60
	0,	// ppid 61
};

static const int u64_in_char = sizeof(u64) / sizeof(char);

static int strace_head_init = 0;

static int raw_init_strace_head(struct exec_context *current)
{
	if (verbose > 2)
		printk("[OS] Initialised strace_head\n");
	current->st_md_base = (struct strace_head *)os_alloc(sizeof(struct strace_head));
	if (current->st_md_base == NULL)
		return -EINVAL;
	current->st_md_base->count = 0;
	current->st_md_base->next = NULL;
	return 0;
}

int perform_tracing(u64 syscall_num, u64 param1, u64 param2, u64 param3, u64 param4)
{
	struct exec_context *current = get_current_ctx();
	struct strace_head *head = current->st_md_base;
	if (syscall_num == 38 || head->is_traced == 0)
		return 0;
	if (head->is_traced != current->pid + 1)
		return 0;
	if (head->tracing_mode == FILTERED_TRACING)
	{
		int found = 0;
		struct strace_info *curr = head->next;
		while (curr)
		{
			if (curr->syscall_num == syscall_num)
			{
				found = 1;
				break;
			}
			curr = curr->next;
		}
		if (!found)
			return 0;
	}

	int num_args = syscall_args[syscall_num];
	if (verbose > 1)
		printk("[OS] [strace] Tracing invoked by syscall %d with num args %d\n", syscall_num, num_args);
	u64 *buf = (u64 *)os_alloc(sizeof(u64) * (1 + num_args));
	*(buf) = syscall_num;
	switch (num_args)
	{
	case 4:
		*(buf + 4) = param4;
	case 3:
		*(buf + 3) = param3;
	case 2:
		*(buf + 2) = param2;
	case 1:
		*(buf + 1) = param1;
	default:
		break;
	}
	int ret_val = raw_trace_buffer_write(current->files[head->strace_fd], (char *)buf, (1 + num_args) * u64_in_char);
	os_free(buf, sizeof(u64) * (1 + num_args));
	return 0;
}

int sys_strace(struct exec_context *current, int syscall_num, int action)
{
	if (verbose > 1)
		printk("[OS] [strace details] strace syscall %d action %d\n", syscall_num, action);
	if (strace_head_init == 0)
	{
		int init_ret = raw_init_strace_head(current);
		if (init_ret == -EINVAL)
			return init_ret;
		current->st_md_base->is_traced = 0;
		strace_head_init = 1;
	}
	struct strace_head *head = current->st_md_base;

	if (action == ADD_STRACE)
	{
		if (head->count >= STRACE_MAX)
			return -EINVAL;

		// check if already exists
		struct strace_info *curr = head->next;
		int count = head->count;
		while (count--)
		{
			if (curr->syscall_num == syscall_num)
				return -EINVAL;
			curr = curr->next;
		}
		// create new strace_info
		struct strace_info *new_info = (struct strace_info *)os_alloc(sizeof(struct strace_info));
		if (new_info == NULL)
			return -EINVAL;
		new_info->syscall_num = syscall_num;
		head->count++;
		new_info->next = head->next;
		head->next = new_info;
		return 0;
	}
	else if (action == REMOVE_STRACE)
	{
		// iterate and remove strace_info
		struct strace_info *curr = head->next;
		struct strace_info *prev = NULL;
		int count = head->count;
		while (count--)
		{
			if (curr->syscall_num == syscall_num)
			{
				// remove this node
				if (prev == NULL)
					head->next = curr->next;
				else
					prev->next = curr->next;
				os_free(curr, sizeof(struct strace_info));
				return 0;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	return -EINVAL;
}

int sys_read_strace(struct file *filep, char *buff, u64 count)
{
	if (verbose > 1)
		printk("[OS] [read_strace] Reading strace for %d calls\n", count);
	// needs buffer to be count * 5 size at least
	// we don't have any idea about number of args in syscall
	// without knowing which syscalls to consider
	int buf_validity = is_valid_mem_range((unsigned long)buff, count * 5 * u64_in_char, O_WRITE);
	if (buf_validity == -EACCES || buf_validity == -EBADMEM)
		return -EINVAL;

	struct exec_context *current = get_current_ctx();
	int strace_fd = current->st_md_base->strace_fd;
	if (current->files[strace_fd] != filep)
	{
		if (verbose > 0)
			printk("[OS] [read_strace] Found filep different than strace_fd\n");
		return -EINVAL;
	}
	char *curr = buff;
	int bytes = 0;
	while (count--)
	{
		trace_buffer_read(filep, curr, u64_in_char); // get current syscall
		u64 syscall_num = *(u64 *)curr;
		if (verbose > 2)
			printk("[OS] [read_strace] read syscall %d from tb\n", syscall_num);
		int num_args = syscall_args[syscall_num];
		if (num_args < 0)
			continue;
		bytes += (1 + num_args) * u64_in_char;

		if (verbose > 2)
			printk("[OS] [read_strace] num arguments %d\n", num_args);
		curr += u64_in_char;
		trace_buffer_read(filep, curr, num_args * u64_in_char);
		curr += num_args * u64_in_char;
	}
	return bytes;
}

int sys_end_strace(struct exec_context *current)
{
	if (current->st_md_base == NULL)
		return -EINVAL;

	struct strace_head *head = current->st_md_base;
	if (head->is_traced == 0)
		return -EINVAL;
	if (head->is_traced != current->pid + 1)
		return 0;
	struct strace_info *curr = head->next;
	struct strace_info *last = head->last;
	while (head->count--)
	{
		os_free(curr, sizeof(struct strace_info));
		curr = curr->next;
	}
	os_free(head, sizeof(struct strace_head));
	head->is_traced = 0;
	return 0;
}

int sys_start_strace(struct exec_context *current, int fd, int tracing_mode)
{
	if (strace_head_init == 0)
	{
		raw_init_strace_head(current);
		strace_head_init = 1;
	}
	struct strace_head *head = current->st_md_base;
	head->is_traced = current->pid + 1;
	head->strace_fd = fd;
	head->tracing_mode = tracing_mode;
	head->last = NULL; // unused
	return 0;
}

/**
 * Start of ftrace functionality
 * */

static int ftrace_head_init = 0;

static int raw_init_ftrace_head(struct exec_context *current)
{
	if (verbose > 2)
		printk("[OS] Initialised ftrace_head\n");
	current->ft_md_base = (struct ftrace_head *)os_alloc(sizeof(struct ftrace_head));
	current->ft_md_base->count = 0;
	current->ft_md_base->last = NULL;
	current->ft_md_base->next = NULL;
}

static void print_traced_functions(struct ftrace_head *head)
{
	struct ftrace_info *curr = head->next;
	int count = head->count;
	while (count--)
	{
		printk("[OS] [LL] faddr %x num_args %d ->\n", curr->faddr, curr->num_args);
		curr = curr->next;
	}
}

long do_ftrace(struct exec_context *ctx, unsigned long faddr, long action, long nargs, int fd_trace_buffer)
{
	if (verbose > 1)
		printk("[OS] [ftrace] called for %x for action: %d\n", faddr, action);
	if (ftrace_head_init == 0)
	{
		raw_init_ftrace_head(ctx);
		ftrace_head_init = 1;
	}
	struct ftrace_head *head = ctx->ft_md_base;
	struct ftrace_info *curr = head->next;
	int count = head->count;

	switch (action)
	{
	case ADD_FTRACE:;
		struct ftrace_info *new_fn = (struct ftrace_info *)os_alloc(sizeof(struct ftrace_info));
		new_fn->faddr = faddr;
		new_fn->capture_backtrace = 0;
		new_fn->num_args = nargs;
		new_fn->fd = fd_trace_buffer;
		new_fn->code_backup[0] = INV_OPCODE;
		new_fn->code_backup[1] = INV_OPCODE;
		new_fn->code_backup[2] = INV_OPCODE;
		new_fn->code_backup[3] = INV_OPCODE;
		if (count >= FTRACE_MAX)
			return -EINVAL;
		while (count--)
		{
			if (curr->faddr == faddr)
				return -EINVAL;
			curr = curr->next;
		}
		if (verbose > 2)
			printk("[OS] [ftrace] Adding function %x\n", faddr);
		// place the new fn
		new_fn->next = head->next;
		head->next = new_fn;
		head->count++;
		return 0;
		break;
	case REMOVE_FTRACE:;
		struct ftrace_info *prev = NULL;
		while (count--)
		{
			if (curr->faddr == faddr)
			{
				if (prev == NULL)
					ctx->ft_md_base->next = curr->next;
				else
					prev->next = curr->next;
				head->count--;
				if (verbose > 2)
					printk("[OS] [ftrace] Removing function %x\n", faddr);
				os_free(curr, sizeof(struct ftrace_info));
				return 0;
			}
			prev = curr;
			curr = curr->next;
		}
		return -EINVAL;
		break;
	case ENABLE_FTRACE:;
		while (count--)
		{
			if (curr->faddr == faddr)
			{
				// modify 4B of the function's first and possibly the
				// second instruction
				for (int i = 0; i < 4; i++)
				{
					*((u8 *)curr->code_backup + i) = *((u8 *)faddr + i);
					*((u8 *)faddr + i) = INV_OPCODE;
				}
				if (verbose > 2)
					printk("[OS] [ftrace] Enabling tracing for function %x\n", faddr);
				return 0;
			}
			curr = curr->next;
		}
		return -EINVAL;
		break;
	case DISABLE_FTRACE:;
		while (count--)
		{
			// address found, and has trace enabled
			// ideally should check for i=0 to 3
			// something like *((u32 *)curr->code_backup) == 0xFFFFFFFF;
			if (curr->faddr == faddr && curr->code_backup[0] != INV_OPCODE)
			{
				for (int i = 0; i < 4; i++)
				{
					*((u8 *)faddr + i) = *((u8 *)curr->code_backup + i);
					// codebackup will always have the correct bytes, if the ENABLE_FTRACE had ever been called
				}
				if (verbose > 2)
					printk("[OS] [ftrace] Disabling tracing for function %x\n", faddr);
				return 0;
			}
			curr = curr->next;
		}
		return -EINVAL;
		break;
	case ENABLE_BACKTRACE:;
		do_ftrace(ctx, faddr, ENABLE_FTRACE, nargs, fd_trace_buffer);
		while (count--)
		{
			if (curr->faddr == faddr)
			{
				if (verbose > 2)
					printk("[OS] [ftrace] Enabling backtrace for function %x\n", faddr);
				curr->capture_backtrace = 1;
				return 0;
			}
			curr = curr->next;
		}
		return -EINVAL;
		break;
	case DISABLE_BACKTRACE:;
		do_ftrace(ctx, faddr, DISABLE_FTRACE, nargs, fd_trace_buffer);
		while (count--)
		{
			if (curr->faddr == faddr)
			{
				if (verbose > 2)
					printk("[OS] [ftrace] Disabling backtrace for function %x\n", faddr);
				curr->capture_backtrace = 0;
				return 0;
			}
			curr = curr->next;
		}
		return -EINVAL;
		break;
	default:
		break;
	}
	return 0;
}

static void save_return_addresses(struct ftrace_info *curr, struct user_regs *regs, u64 rbp)
{
	struct exec_context *ctx = get_current_ctx();
	u64 return_addr = *((u64 *)rbp + 1);
	u64 prev_rbp = *((u64 *)rbp);
	if (verbose > 0)
		printk("[OS] [Fault] found %x return addr\n", return_addr);
	if (return_addr == END_ADDR)
		return;
	u64 *buf = (u64 *)os_alloc(sizeof(u64) * 1);
	*(buf) = return_addr;
	int ret_val = raw_trace_buffer_write(ctx->files[curr->fd], (char *)buf, u64_in_char);
	os_free(buf, sizeof(u64) * 1);
	// write the END_ADDR return value as well, ease in reading
	save_return_addresses(curr, regs, prev_rbp);
}

long handle_ftrace_fault(struct user_regs *regs)
{
	struct exec_context *ctx = get_current_ctx();
	struct ftrace_head *head = ctx->ft_md_base;
	struct ftrace_info *curr = head->next;
	int count = head->count;
	if (verbose > 1)
		printk("[OS] [Fault] entry_rip: %x\n", regs->entry_rip);
	// simulate the first 4 bytes of the function
	// push %rbp
	// regs->entry_rsp -= (u64)1;
	regs->entry_rsp = regs->entry_rsp - (u64)8;
	*((u64 *)(regs->entry_rsp)) = regs->rbp;
	// mov %rsp, %rbp
	// regs->rbp = regs->entry_rsp;
	regs->rbp = regs->entry_rsp;

	while (count--)
	{
		if (curr->faddr == regs->entry_rip)
		{
			// got the function in linked list
			break;
		}
		curr = curr->next;
	}
	if (curr == NULL)
	{
		if (verbose > 0)
			printk("[OS] [Fault] Problem! Didn't find function %x in linked list\n", regs->entry_rip);
		return 0;
	}
	int num_args = curr->num_args;
	u64 *buf = (u64 *)os_alloc(sizeof(u64) * (1 + num_args));
	*(buf) = curr->faddr;
	switch (num_args)
	{
	case 5:
		*(buf + 5) = regs->r8;
	case 4:
		*(buf + 4) = regs->rcx;
	case 3:
		*(buf + 3) = regs->rdx;
	case 2:
		*(buf + 2) = regs->rsi;
	case 1:
		*(buf + 1) = regs->rdi;
	default:
		break;
	}
	int ret_val = raw_trace_buffer_write(ctx->files[curr->fd], (char *)buf, (1 + num_args) * u64_in_char);
	os_free(buf, sizeof(u64) * (1 + num_args));

	buf = (u64 *)os_alloc(sizeof(u64) * 1);
	// check for backtracing now
	if (curr->capture_backtrace == 1)
	{
		// saving the first instr of function
		if (verbose > 2)
			printk("[OS] [Fault] backtrace on for %x\n", curr->faddr);
		*(buf) = regs->entry_rip;
		if (verbose > 2)
			printk("[OS] [Fault] saving %x current instr addr\n", *buf);
		int ret_val = raw_trace_buffer_write(ctx->files[curr->fd], (char *)buf, u64_in_char);
		// saving rest of return addresses
		save_return_addresses(curr, regs, regs->rbp);
	}
	else
	{
		if (verbose > 2)
			printk("[OS] [Fault] backtrace off for %x\n", curr->faddr);
	}
	*(buf) = END_ADDR;
	if (verbose > 2)
		printk("[OS] [Fault] saving %x delim return addr\n", *buf);
	ret_val = raw_trace_buffer_write(ctx->files[curr->fd], (char *)buf, u64_in_char);
	os_free(buf, sizeof(u64) * 1);

	// now store details of this function call in trace_buffer
	// make the instruction pointer back to the next intruction
	regs->entry_rip = regs->entry_rip + (u64)4;
	if (verbose > 1)
		printk("[OS] [Fault] switching entry_rip to: %x\n", regs->entry_rip);
	return 0;
}

int sys_read_ftrace(struct file *filep, char *buff, u64 count)
{
	if (verbose > 1)
		printk("[OS] [read_ftrace] Reading ftrace for %d functions\n", count);
	int buf_validity = is_valid_mem_range((unsigned long)buff, count * 6 * u64_in_char, O_WRITE);
	if (buf_validity == -EACCES || buf_validity == -EBADMEM)
		return -EINVAL;

	struct exec_context *ctx = get_current_ctx();
	struct ftrace_head *head = ctx->ft_md_base;
	char *curr_buff = buff;
	int bytes = 0;
	while (count--)
	{
		int fncount = head->count;
		struct ftrace_info *curr = head->next;
		int read_val = trace_buffer_read(filep, curr_buff, u64_in_char); // get current function address
		if (read_val == 0)
			return bytes;
		curr_buff += u64_in_char;
		u64 faddr = *(u64 *)curr;
		while (fncount--)
		{
			if (faddr == curr->faddr)
				break;
			curr = curr->next;
		}
		if (curr == NULL)
		{
			if (verbose > 0)
				printk("[OS] [read_ftrace] read faddr not found in curr\n");
			return -EINVAL;
		}

		if (verbose > 2)
			printk("[OS] [read_ftrace] read faddr %x from tb\n", faddr);
		int num_args = curr->num_args;
		if (num_args < 0)
			continue;

		if (verbose > 2)
			printk("[OS] [read_ftrace] num arguments %d\n", num_args);
		trace_buffer_read(filep, curr_buff, num_args * u64_in_char);
		curr_buff += num_args * u64_in_char;
		bytes += (1 + num_args) * u64_in_char;

		// NOTE THE MOST UNSAFE CODE EVER
		// Guaranteed correctness only when fault_handler as above
		// inserts an END_ADDR at end per function tracing
		while (1)
		{
			char tmp_buff[4];
			// read a byte
			// check if it's equal to END_ADDR
			raw_trace_buffer_read(filep, tmp_buff, u64_in_char);
			u64 obtained_addr = *((u64 *)tmp_buff);
			if (obtained_addr == END_ADDR)
				break;
			if (verbose > 2)
				printk("[OS] [read_ftrace] obtained backtrace %x\n", obtained_addr);
			*((u64 *)curr_buff) = obtained_addr;
			curr_buff += 1 * u64_in_char;
			bytes += u64_in_char;
		}
	}
	return bytes;
}
