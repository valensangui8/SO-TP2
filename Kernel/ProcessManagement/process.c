#include <process.h>
#include <stdint.h>

void init_procces(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, void *stack_pointer, char foreground, char **argv, int argc) {
	process->pid = pid;
	process->ppid = ppid;
	process->priority = priority;
	process->state = state;
	process->stack_pointer = stack_pointer;
	process->foreground = foreground;
	for (int i = 0; i < 32; i++) { // revisar como ponemos el nombre
		process->name[i] = name[i];
	}
	process->is_active = 1;
	process->argv = argv;
	process->argc = argc;
}


uint8_t has_children(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active != 0 && scheduler->processes[i].ppid == pid) {
			return 1;
		}
	}
	return 0;
}

void wait_children(unsigned int ppid) {
	if (has_children(ppid)) {
		SchedulerInfo scheduler = get_scheduler();
		for (int i = 0; i < MAX_PROCESS; i++) {
			if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].ppid == ppid) {
				while (scheduler->processes[i].state != DEAD) {
					// wait
				}
			}
		}
	}
}

uint8_t kill_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].pid == pid) {
			scheduler->processes[i].state = DEAD;
			scheduler->processes[i].is_active = 0;
			return 1;
		}
	}
	return 0;
}



void update_priority(unsigned int pid, Priority new_priority) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].pid == pid) {
			scheduler->processes[i].priority = new_priority;
			return;
		}
	}
}

void block_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].pid == pid) {
			scheduler->processes[i].state = BLOCKED;
			return;
		}
	}
}

void unblock_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active == 1 && scheduler->processes[i].pid == pid) {
			scheduler->processes[i].state = READY;
			return;
		}
	}
}

void yield() {
	SchedulerInfo scheduler = get_scheduler();
	scheduler->quantum_remaining = 0;
}

