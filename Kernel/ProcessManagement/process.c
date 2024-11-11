// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <process.h>

static int argsLen(char **array) {
	int i = 0;
	while (array[i] != NULL)
		i++;
	return i;
}

static void exit_process(int ret, unsigned int pid) {
	PCBT *process = find_process(pid);
	process->ret = ret;
	kill_process(pid);
	yield();
}

void process_function(main_function rip, char **argv, uint64_t argc) {
	int new_argc = argsLen(argv);
	int ret = rip(new_argc, argv);
	exit_process(ret, get_pid());
}

static void assign_fd(PCBT *process, int16_t index, int16_t fd, char mode) {
	process->fds[index] = fd;
	if (fd >= BUILT_IN_FD) {
		open_pipe(fd, mode, process->pid);
	}
}

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, char **argv, int argc, main_function rip, const int16_t fds[]) {
	process->pid = pid;
	process->ppid = ppid;
	process->waiting_pid = NO_CHILDREN;
	process->priority = priority;
	process->times_to_run = priority;
	assign_fd(process, STDIN, fds[STDIN], 'r');
	assign_fd(process, STDOUT, fds[STDOUT], 'w');
	assign_fd(process, STDERR, fds[STDERR], 'w');

	process->stack_base = alloc_memory(STACK_SIZE);
	if (process->stack_base == NULL) {
		draw_with_color("Error: Could not allocate memory for process", 0xFF0000);
		free_memory(process->stack_base);
		return;
	}
	void *stackEnd = (void *) ((uint64_t) process->stack_base + STACK_SIZE);
	my_strncpy(process->name, name, sizeof(process->name));
	process->argv = argv;
	process->argc = argc;
	process->stack_pointer = _initialize_stack_frame(&process_function, rip, stackEnd, (void *) process->argv);
}

int64_t wait_children(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	if (pid == INIT_PID) { // Has no father
		return -1;
	}
	PCBT *child = find_process(pid);
	PCBT *parent = find_process(child->ppid);

	if (child->state != ZOMBIE) {
		parent->waiting_pid = pid;
		block_process(parent->pid);
		yield();
	}

	child->state = DEAD;
	scheduler->amount_processes--;

	return child->ret;
}

void yield() {
	_yield();
}

char *process_state(PCBT process) {
	static char status[10];
	my_strcpy(status, "");

	switch (process.state) {
		case BLOCKED:
			my_strcat(status, "T");
			break;
		case READY:
			my_strcat(status, "S");
			break;
		case RUNNING:
			my_strcat(status, "R");
			break;
		case ZOMBIE:
			my_strcat(status, "Z");
			break;
		default:
			my_strcat(status, "UNKNOWN");
			return status;
	}
	switch (process.priority) {
		case PRIORITY3:
			my_strcat(status, "H");
			break;
		case PRIORITY2:
			my_strcat(status, "M");
			break;
		case PRIORITY4:
			my_strcat(status, "<");
			break;
		case PRIORITY1:
			my_strcat(status, "N");
			break;
	}
	if (process.fds[STDIN] == DEV_NULL) { // Background
		my_strcat(status, "+");
	}
	if (process.pid == SESSION_LEADER) { // Session leader
		my_strcat(status, "s");
	}
	my_strcat(status, "\0");
	return status;
}

// change to find_process
void process_status(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	command_enter();
	draw_word("STAT - T: Blocked - S: Ready  - R: Running - Z: Zombie - <: Top priority - N: Lowest priority - +: Background - s: Session leader");
	command_enter();
	draw_word("PID        STAT          RSP           RBP         COMMAND");
	command_enter();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].pid == pid) {
			draw_int(scheduler->processes[i].pid);
			draw_word("        ");
			draw_word(process_state(scheduler->processes[i]));
			draw_word("        ");
			draw_hex((uint64_t) scheduler->processes[i].stack_pointer);
			draw_word("        ");
			draw_hex((uint64_t) scheduler->processes[i].stack_base);
			draw_word("        ");
			draw_word(scheduler->processes[i].name);
			command_enter();
		}
	}
	enter();
}

unsigned int get_pid() {
	SchedulerInfo scheduler = get_scheduler();
	return scheduler->current_pid;
}

unsigned int get_ppid() {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = &(scheduler->processes[scheduler->index_rr]);
	return process->ppid;
}

void close_file_descriptor(int16_t fd, int pid) {
	PCBT *process = find_process(get_pid());
	if (process->fds[fd] >= BUILT_IN_FD) {
		close_pipe(process->fds[fd], pid);
	}
}