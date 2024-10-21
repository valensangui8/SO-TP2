#include <process.h>


static int argsLen(char **array) {
    int i = 0;
    while (array[i] != NULL) i++;
    return i;
}

void process_function(main_function rip, char **argv, uint64_t argc) {
	
	int new_argc = argsLen(argv);
	drawInt(new_argc);
    int ret = rip(new_argc, argv);

    kill_process(get_pid());

    yield();

    while (1) {
        _hlt();
    }
}



void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, char foreground, char **argv, int argc, main_function rip) {
	process->pid = pid;
	process->ppid = ppid;
	process->priority = priority;
	process->foreground = foreground;
	process->times_to_run = priority;
	process->stack_base = alloc_memory(STACK_SIZE);
	if (process->stack_base == NULL) {
        free_memory(process->stack_base);
        return NULL;
    }
	void *stackEnd = (void *) ((uint64_t) process->stack_base + STACK_SIZE);
	my_strncpy(process->name, name, sizeof(process->name));
	process->argv = argv;
	process->argc = argc;
	process->stack_pointer = _initialize_stack_frame(&process_function, rip, stackEnd,(void *) process->argv);

}

uint8_t has_children(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].ppid == pid && scheduler->processes[i].state != DEAD) {
			return 1;
		}
	}
	return 0;
}

int64_t wait_children(unsigned int ppid) {
	if (has_children(ppid)) {
		SchedulerInfo scheduler = get_scheduler();
		for (int i = 0; i < MAX_PROCESS; i++) {
			if (scheduler->processes[i].ppid == ppid && scheduler->processes[i].state != DEAD) {
				while (scheduler->processes[i].state != DEAD) {
					// wait
				}
			}
		}
	}
	return 0;
}

void yield() {
	SchedulerInfo scheduler = get_scheduler();
	scheduler->quantum_remaining = 0;
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
	if (process.foreground) {
		my_strcat(status, "+"); 
	}
	else if (process.pid == SESSION_LEADER) {
		my_strcat(status, "s"); 
	}
	my_strcat(status, "\0");
	return status;
}

void process_status(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	commandEnter();
	drawWord("PID        STAT          RSP           RBP         COMMAND");
	commandEnter();
	for (int i = 0; i < scheduler->amount_processes; i++) {
		if (scheduler->processes[i].pid == pid) {
			drawInt(scheduler->processes[i].pid);
			drawWord("        ");
			drawWord(process_state(scheduler->processes[i]));
			drawWord("        ");
			drawHex(scheduler->processes[i].stack_pointer);
			drawWord("        ");
			drawHex(scheduler->processes[i].stack_base);
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