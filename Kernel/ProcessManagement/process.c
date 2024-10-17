#include <process.h>


void process_function(main_function rip, char **argv, uint64_t argc) {
	
    int ret = rip(argv, argc);

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
	process->stack_base = alloc_memory(4096);
	if (process->stack_base == NULL) {
        free_memory(process->stack_base);
        return NULL;
    }
	void *stackEnd = (void *) ((uint64_t) process->stack_base + 4096);
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

uint8_t kill_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].pid == pid && scheduler->processes[i].state != DEAD) {
			scheduler->processes[i].state = DEAD;
			scheduler->amount_processes--;
			free_memory(scheduler->processes[i].stack_base);
			free_memory(scheduler->processes[i].argv);
			return 1;
		}
	}
	return 0;
}

void update_priority(unsigned int pid, Priority new_priority) {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = NULL;

	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].pid == pid && scheduler->processes[i].state != DEAD) {
			process = &(scheduler->processes[i]);
			break;
		}
	}
	if (process == NULL || process->priority == new_priority) {
		return;
	}

	process->times_to_run = new_priority;

	process->priority = new_priority;
}

uint16_t block_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].pid == pid && scheduler->processes[i].state != DEAD) {
			if (scheduler->processes[i].state == ZOMBIE || scheduler->processes[i].state == BLOCKED) {
				return 0;
			}
			scheduler->processes[i].state = BLOCKED;
			return 1;
		}
	}
	
	return 0;
}

uint16_t unblock_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if ( scheduler->processes[i].pid == pid && scheduler->processes[i].state == BLOCKED) {
			scheduler->processes[i].state = READY;
			return 1;
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
