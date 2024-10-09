#include "scheduler.h"

SchedulerInfo create_scheduler() {
	SchedulerInfo scheduler = (SchedulerInfo) SCHEDULER_ADDRESS;
	for (int i = 0; i < MAX_PROCESS; i++) {
		scheduler->processes[i] = NULL; // No process when initialized
	}

	for (int i = 0; i < MAX_PROCESS * PRIORITY4; i++) {
		scheduler->round_robin[i] = NULL; // No process when initialized
	}

	scheduler->current_pid = 0;
	scheduler->index_p = 0;
	scheduler->index_rr = 0;
	scheduler->quantum_remaining = QUANTUM;

	return scheduler;
}

SchedulerInfo get_scheduler() {
	return (SchedulerInfo) SCHEDULER_ADDRESS;
}

int8_t set_state(uint8_t new_state) {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = scheduler->round_robin[scheduler->index_rr];
	if (process == NULL) {
		return -1;
	}
	PCBState old_state = process->state;
	if (new_state == ZOMBIE || new_state == RUNNING || old_state == ZOMBIE) {
		return -1;
	}
	process->state = new_state;
	if (new_state == old_state) {
		return old_state;
	}

	return new_state;
}

int8_t get_status() {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = scheduler->round_robin[scheduler->index_rr];
	if (process == NULL) {
		return -1;
	}
	return process->state;
}

void add_process(PCBT *process) {
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i] == NULL) {
			scheduler->processes[i] = process;
			process->state = READY;

			int start_index = scheduler->index_rr;
			int priority = process->priority;
			int inserted = 0;
			// Round robin insertion
			for (int j = 0; j < MAX_PROCESS * PRIORITY4 && inserted < priority; j++) {
				int index = (start_index + j) % (MAX_PROCESS * PRIORITY4);

				if (scheduler->round_robin[index] == NULL || scheduler->round_robin[index]->state == DEAD) { // Place the process in the free slot
					scheduler->round_robin[index] = process;
					inserted++;

					if (inserted == priority) {
						scheduler->index_rr = (index + 1) % (MAX_PROCESS * PRIORITY4);
					}
				}
			}
			scheduler->index_p = i;
			return;
		}
	}
	// manejo si esta lleno --> pensarlo
}
