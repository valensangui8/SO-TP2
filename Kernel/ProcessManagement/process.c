#include <process.h>

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, char foreground, char **argv, int argc, uint64_t rip) {
	process->pid = pid;
	process->ppid = ppid;
	process->priority = priority;
	process->state = state;
	process->foreground = foreground;
	my_strncpy(process->name, name, sizeof(process->name));
	process->stack_process = load_stack_pointer(rip, process->stack_process->rsp, argv, argc);
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
