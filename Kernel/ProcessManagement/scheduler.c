#include "scheduler.h"

static void update_index_p(SchedulerInfo scheduler);
static char **alloc_arguments(char **argv, uint64_t argc);

static int initialized = 0;

SchedulerInfo create_scheduler() {
	SchedulerInfo scheduler = (SchedulerInfo) SCHEDULER_ADDRESS;
	for (int i = 0; i < MAX_PROCESS; i++) {
		scheduler->processes[i].is_active = 0; 
	}
	for (int i = 0; i < MAX_PROCESS * PRIORITY4; i++) {
		scheduler->round_robin[i] = NULL; 
	}
	scheduler->current_pid = 0;
	scheduler->index_p = 0;
	scheduler->index_rr = 0;
	scheduler->quantum_remaining = QUANTUM;
	scheduler->amount_processes = 0;
	scheduler->next_pid = 1;
	initialized = 1;
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

uint16_t create_process(char *name, uint16_t ppid, Priority priority, char foreground, char *argv[], int argc, main_function rip) {
	SchedulerInfo scheduler = get_scheduler();
	scheduler->amount_processes++;

	int free_spot = scheduler->index_p;
	if (free_spot == -1) {
		return -1;
	}
	PCBT *process = &(scheduler->processes[free_spot]);
	process->is_active = 1;
	
	update_index_p(scheduler);
	char **new_argv = alloc_arguments(argv, argc);
	process->pid = scheduler->next_pid++;

	init_process(process, name, process->pid, ppid, priority, foreground, new_argv, argc, rip);


	if(process->pid ==1){
		scheduler->index_rr = 0;
	}
	int start_index = scheduler->index_rr;
	int process_priority = process->priority;
	int inserted = 0;
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
	return process->pid;
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
    PCBT *current_process = scheduler->round_robin[scheduler->index_rr];

    if (scheduler->quantum_remaining == 0 || current_process->state == BLOCKED) {
        if (current_process->state == RUNNING) {
            current_process->state = READY;
			current_process->stack_pointer = stack_pointer;
        }

        do {
            scheduler->index_rr = (scheduler->index_rr + 1) % (MAX_PROCESS * PRIORITY4);
            current_process = scheduler->round_robin[scheduler->index_rr];
        } while (current_process == NULL || current_process->state != READY);

        scheduler->quantum_remaining = QUANTUM;
    } else {
        scheduler->quantum_remaining--;
    }

    current_process->state = RUNNING;

    return current_process;
}


void *scheduler(void *stack_pointer) {
    SchedulerInfo scheduler = get_scheduler();
    if (!initialized) {
        return stack_pointer;
    }

    PCBT *current_process = scheduler->round_robin[scheduler->index_rr];
    if (current_process == NULL || current_process->is_active == 0 || current_process->state == DEAD) {
        scheduler->index_rr = (scheduler->index_rr + 1) % (MAX_PROCESS * PRIORITY4);
        return stack_pointer;
    }

    current_process->stack_pointer = stack_pointer;

    current_process = update_quantum(stack_pointer);



    scheduler->current_pid = current_process->pid;

    return current_process->stack_pointer;
}



unsigned int get_pid() {
	SchedulerInfo scheduler = get_scheduler();
	return scheduler->current_pid;
}

unsigned int get_ppid() {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = scheduler->round_robin[scheduler->index_rr];
	return process->ppid;
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

void list_processes_state() {
	SchedulerInfo scheduler = get_scheduler();
	commandEnter();
	drawWord("PID        STAT          RSP           RBP         COMMAND");
	commandEnter();
	for (int i = 0; i < scheduler->amount_processes; i++) {
		drawInt(scheduler->processes[i].pid);
		drawWord("        ");
		drawWord(process_state(scheduler->processes[i]));
		drawWord("        ");
		drawHex(scheduler->processes[i].stack_process->rsp);
		drawWord("        ");
		drawHex(scheduler->processes[i].stack_process->my_registers.rbp);
		drawWord("        ");
		drawWord(scheduler->processes[i].name);
		commandEnter();
	}
	enter();
}