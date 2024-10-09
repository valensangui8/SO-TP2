#include <process.h>
#include <stdint.h>

void init_procces(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, int16_t file_descriptors[], void *stack_base, void *stack_pointer) {
	process->pid = pid;
	process->ppid = ppid;
	process->priority = priority;
	process->state = state;
	process->stack_base = stack_base;
	process->stack_pointer = stack_pointer;
	for (int i = 0; i < DESCRIPTORS; i++) {
		process->file_descriptors[i] = file_descriptors[i];
	}
	for (int i = 0; i < 32; i++) { // revisar como ponemos el nombre
		process->name[i] = name[i];
	}
}

uint8_t has_children(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i] != NULL && scheduler->processes[i]->ppid == pid) {
			return 1;
		}
	}
	return 0;
}

void wait_children(unsigned int ppid) {
	if (has_children(ppid)) {
		SchedulerInfo scheduler = get_scheduler();
		for (int i = 0; i < MAX_PROCESS; i++) {
			if (scheduler->processes[i] != NULL && scheduler->processes[i]->ppid == ppid) {
				while (scheduler->processes[i]->state != DEAD) {
					// wait
				}
			}
		}
	}
}

uint8_t kill_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i] != NULL && scheduler->processes[i]->pid == pid) {
			scheduler->processes[i]->state = DEAD;
			// recursos liberar del vector
			return 1;
		}
	}
	return 0;
}
