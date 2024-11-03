#include <process.h>

static int argsLen(char **array) {
    int i = 0;
    while (array[i] != NULL) i++;
    return i;
}

static void exit_process(int ret, unsigned int pid) {
	PCBT *process = find_process(pid);
	process->ret = ret;
	kill_process(pid);
	close_pipe(process->fds[STDIN], process->pid);
	close_pipe(process->fds[STDOUT], process->pid);
	close_pipe(process->fds[STDERR], process->pid);
	yield();
}

void process_function(main_function rip, char **argv, uint64_t argc) {
	int new_argc = argsLen(argv);
    int ret = rip(new_argc, argv);
	exit_process(ret, get_pid());

}

static void assign_fd(PCBT *process, int16_t index, int16_t fd, char mode){
	process->fds[index]= fd;
	if(fd>=BUILT_IN_FD){
		open_pipe(fd, mode);
	}
}

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, char **argv, int argc, main_function rip, int16_t fds[]) {
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
        free_memory(process->stack_base);
        return;
    }
	void *stackEnd = (void *) ((uint64_t) process->stack_base + STACK_SIZE);
	my_strncpy(process->name, name, sizeof(process->name));
	process->argv = argv;
	process->argc = argc;
	process->stack_pointer = _initialize_stack_frame(&process_function, rip, stackEnd,(void *) process->argv);
	
}

int64_t wait_children(unsigned int pid) {
	if(pid == INIT_PID){ // Has no father
		return -1;
	}
	PCBT *child = find_process(pid);
	PCBT *parent = find_process(child->ppid);
	
	if(child->state != ZOMBIE) {
		parent->waiting_pid = pid;
		block_process(parent->pid);
		yield();
	} 
	
	child->state = DEAD;
	

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
	if (process.priority == PRIORITY4) {
		my_strcat(status, "<"); 
	}
	else if (process.priority == PRIORITY1) {
		my_strcat(status, "N"); 
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
	commandEnter();
	drawWord("STAT - T: Blocked - S: Ready  - R: Running - Z: Zombie - <: Top priority - N: Lowest priority - +: Background - s: Session leader");
	commandEnter();
	drawWord("PID        STAT          RSP           RBP         COMMAND");
	commandEnter();
	for (int i = 0; i < scheduler->amount_processes; i++) {
		if (scheduler->processes[i].pid == pid) {
			drawInt(scheduler->processes[i].pid);
			drawWord("        ");
			drawWord(process_state(scheduler->processes[i]));
			drawWord("        ");
			drawHex((uint64_t) scheduler->processes[i].stack_pointer);
			drawWord("        ");
			drawHex((uint64_t) scheduler->processes[i].stack_base);
			drawWord("        ");
			drawWord(scheduler->processes[i].name);
			commandEnter();
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