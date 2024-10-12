#include "scheduler.h"

SchedulerInfo create_scheduler() {
	SchedulerInfo scheduler = (SchedulerInfo) SCHEDULER_ADDRESS;
	for (int i = 0; i < MAX_PROCESS; i++) {
		scheduler->processes[i].is_active = 0 ; // No process when initialized
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

int8_t get_state() {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = scheduler->round_robin[scheduler->index_rr];
	if (process == NULL) {
		return -1;
	}
	return process->state;
}

void create_process(char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, void *stack_pointer, char foreground, char *argv[], int argc) {
	SchedulerInfo scheduler = get_scheduler();

	int free_spot = scheduler->index_p;
	if( free_spot == -1){
		return;
	}
	PCBT *process = &(scheduler->processes[free_spot]);
	update_index_p(scheduler);
	init_procces(process, name, pid, ppid, priority, state, stack_pointer, foreground, argv, argc);
	int start_index = scheduler->index_rr;
	int priority = process->priority;
	int inserted = 0;
	// Round robin insertion
	for (int j = 0; j < MAX_PROCESS * PRIORITY4 && inserted < priority; j++) {
		int index = (start_index + j) % (MAX_PROCESS * PRIORITY4);
		if (scheduler->round_robin[index] == NULL || scheduler->round_robin[index]->is_active == 0 || scheduler->round_robin[index]->state == DEAD) { 
			scheduler->round_robin[index] = process;
			inserted++;
			if (inserted == priority) {
				scheduler->index_rr = (index + 1) % (MAX_PROCESS * PRIORITY4);

			}
		}
	}
	return;
		
}

static void update_index_p(SchedulerInfo scheduler) {
	int index = scheduler->index_p;
	if (index == -1) {
		return;
	}
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].is_active == 0) {
			scheduler->index_p = i;
			return;
		}
	}
	scheduler->index_p = -1;
}


void update_quantum(void *stack_pointer) {
	SchedulerInfo scheduler = get_scheduler();
	if (scheduler->quantum_remaining == 0 || scheduler->round_robin[scheduler->index_rr]->state == BLOCKED) {
		if(scheduler->round_robin[scheduler->index_rr]->state == RUNNING){
			scheduler->round_robin[scheduler->index_rr]->state = READY;
			scheduler->round_robin[scheduler->index_rr]->stack_pointer = stack_pointer;
		}
		scheduler->index_rr = (scheduler->index_rr + 1) % (MAX_PROCESS * PRIORITY4);
		scheduler->quantum_remaining = QUANTUM;
		while(scheduler->round_robin[scheduler->index_rr]->state != READY){
			scheduler->index_rr = (scheduler->index_rr + 1) % (MAX_PROCESS * PRIORITY4);
		}
		return;
	}
	scheduler->quantum_remaining--;
}

unsigned int get_pid(){
	SchedulerInfo scheduler = get_scheduler();
	return scheduler->current_pid;
}


void *scheduler(void *stack_pointer){
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = scheduler->round_robin[scheduler->index_rr];
	if (process == NULL) {
		return;
	}

	load_stack_pointer(process->stack_pointer);

	scheduler->current_pid = process->pid;
	
	update_quantum(stack_pointer);

	if (process->state == DEAD) {
		scheduler->round_robin[scheduler->index_rr] = NULL;
		scheduler->index_rr = (scheduler->index_rr + 1) % (MAX_PROCESS * PRIORITY4);
		return;
	}
	if (process->state == BLOCKED) {
		scheduler->index_rr = (scheduler->index_rr + 1) % (MAX_PROCESS * PRIORITY4);
		return;
	}
	if (process->state == READY) {
		process->state = RUNNING;
	}
	// Call process
	// update_quantum();
	return process->stack_pointer;
}
