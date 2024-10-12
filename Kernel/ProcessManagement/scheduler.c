#include "scheduler.h"

static void update_index_p(SchedulerInfo scheduler);
static char **alloc_arguments(char **argv, uint64_t argc);

SchedulerInfo create_scheduler() {
	SchedulerInfo scheduler = (SchedulerInfo) SCHEDULER_ADDRESS;
	for (int i = 0; i < MAX_PROCESS; i++) {
		scheduler->processes[i].is_active = 0; // No process when initialized
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

void create_process(char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, char foreground, char *argv[], int argc, main_function rip) {
	SchedulerInfo scheduler = get_scheduler();

	int free_spot = scheduler->index_p;
	if (free_spot == -1) {
		return;
	}
	PCBT *process = &(scheduler->processes[free_spot]);
	update_index_p(scheduler);
	char **new_argv = alloc_arguments(argv, argc);
	init_process(process, name, pid, ppid, priority, state, foreground, new_argv, argc, rip);
	int start_index = scheduler->index_rr;
	int process_priority = process->priority;
	int inserted = 0;
	// Round robin insertion
	for (int j = 0; j < MAX_PROCESS * PRIORITY4 && inserted < process_priority; j++) {
		int index = (start_index + j) % (MAX_PROCESS * PRIORITY4);
		if (scheduler->round_robin[index] == NULL || scheduler->round_robin[index]->is_active == 0 || scheduler->round_robin[index]->state == DEAD) {
			scheduler->round_robin[index] = process;
			inserted++;
			if (inserted == process_priority) {
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

PCBT *update_quantum(void *stack_pointer) {
	SchedulerInfo scheduler = get_scheduler();
	if (scheduler->quantum_remaining == 0 || scheduler->round_robin[scheduler->index_rr]->state == BLOCKED) {
		if (scheduler->round_robin[scheduler->index_rr]->state == RUNNING) {
			scheduler->round_robin[scheduler->index_rr]->state = READY;
		}
		scheduler->round_robin[scheduler->index_rr]->stack_process = get_snapshot(scheduler->round_robin[scheduler->index_rr]->stack_process, stack_pointer);
		scheduler->index_rr = (scheduler->index_rr + 1) % (MAX_PROCESS * PRIORITY4);
		scheduler->quantum_remaining = QUANTUM;
		while (scheduler->round_robin[scheduler->index_rr]->state != READY) {
			if (scheduler->round_robin[scheduler->index_rr]->state == DEAD) {
				scheduler->round_robin[scheduler->index_rr] = NULL;
			}
			scheduler->index_rr = (scheduler->index_rr + 1) % (MAX_PROCESS * PRIORITY4);
		}
	}
	else {
		scheduler->quantum_remaining--;
	}
	return scheduler->round_robin[scheduler->index_rr];
}

uint64_t scheduler(void *stack_pointer) {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = scheduler->round_robin[scheduler->index_rr];
	drawWord(process->name);
	if (process == NULL) {
		return (uint64_t) stack_pointer;
	}
	process = update_quantum(stack_pointer);
	scheduler->current_pid = process->pid;

	return process->stack_process->rsp;
}

unsigned int get_pid() {
	SchedulerInfo scheduler = get_scheduler();
	return scheduler->current_pid;
}

// https://github.com/avilamowski/TP2_SO/blob/master/Kernel/processes/process.c#L66
static char **alloc_arguments(char **argv, uint64_t argc) {
	int totalArgsLen = 0;
	int argsLen[argc];
	for (int i = 0; i < argc; i++) {
		argsLen[i] = my_strlen(argv[i]) + 1;
		totalArgsLen += argsLen[i];
	}
	char **newArgsArray = (char **) alloc_memory(totalArgsLen + sizeof(char **) * (argc + 1));
	char *charPosition = (char *) newArgsArray + (sizeof(char **) * (argc + 1));
	for (int i = 0; i < argc; i++) {
		newArgsArray[i] = charPosition;
		my_memcpy(charPosition, argv[i], argsLen[i]);
		charPosition += argsLen[i];
	}
	newArgsArray[argc] = NULL;
	return newArgsArray;
}