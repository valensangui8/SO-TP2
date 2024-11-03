#include "scheduler.h"

static char **alloc_arguments(char **argv, uint64_t argc);
static void update_index_p(SchedulerInfo scheduler);

static int initialized = 0;

SchedulerInfo create_scheduler() {
	SchedulerInfo scheduler = (SchedulerInfo) SCHEDULER_ADDRESS;
	for (int i = 0; i < MAX_PROCESS; i++) {
		scheduler->processes[i].state = DEAD;
	}
	scheduler->current_pid = 1;
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

uint8_t set_state(uint8_t new_state) {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = &(scheduler->processes[scheduler->index_rr]);
	PCBState old_state = process->state;
	if (new_state == ZOMBIE || new_state == RUNNING || old_state == ZOMBIE) {
		return -1;
	}
	process->state = new_state;
	if (new_state == old_state) {
		return old_state;
	}else if(new_state == BLOCKED){
		block_process(get_pid());
	}
	return new_state;
}

uint8_t get_state() {
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = &(scheduler->processes[scheduler->index_rr]);
	return process->state;
}

uint64_t create_process(char *name, Priority priority, char *argv[], int argc, main_function rip, const int16_t fds[]) {
	SchedulerInfo scheduler = get_scheduler();

	int free_spot = scheduler->index_p;
	if (scheduler->amount_processes == MAX_PROCESS || free_spot == -1) {
		return -1;
	}
	scheduler->amount_processes++;
	PCBT *process = &(scheduler->processes[free_spot]);
	process->state = READY;
	
	update_index_p(scheduler);
	char **new_argv = alloc_arguments(argv, argc);
	int ppid;
	if(scheduler->next_pid == INIT_PID) {
		process->pid = INIT_PID;
		ppid = process->pid;
		scheduler->next_pid++;
	} else {
		process->pid = scheduler->next_pid++;
		ppid = get_pid();
	}
	
	init_process(process, name, process->pid, ppid, priority, new_argv, argc, rip, fds);

	PCBT * parent = find_process(ppid);
	parent->waiting_pid++;

	return process->pid;
}

static void update_index_p(SchedulerInfo scheduler) {
	int index = scheduler->index_p;
	if (index == -1) {
		return;
	}
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].state == DEAD) {
			scheduler->index_p = i;
			return;
		}
	}
	scheduler->index_p = -1;
}

static void collect_zombies(){
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].state == ZOMBIE && scheduler->processes[i].ppid == INIT_PID) {
			wait_children(scheduler->processes[i].pid);
		}
	}
}

PCBT *update_quantum(void *stack_pointer) {
    SchedulerInfo scheduler = get_scheduler();
    PCBT *current_process = &(scheduler->processes[scheduler->index_rr]);

    if (scheduler->quantum_remaining == 0 || current_process->state == BLOCKED || current_process->state == ZOMBIE || current_process->times_to_run == 0) {
		if(scheduler->quantum_remaining == 0){
			collect_zombies();
		}
        if (current_process->state == RUNNING) {
            current_process->state = READY;
        }

        if (current_process->state != BLOCKED && current_process->times_to_run > 0) {
            current_process->times_to_run--;
        }
		current_process->stack_pointer = stack_pointer; // Save the stack pointer 

		while (current_process->state != READY || current_process->times_to_run == 0){
			scheduler->index_rr = (scheduler->index_rr + 1) % MAX_PROCESS; 
            current_process = &(scheduler->processes[scheduler->index_rr]);

            if (current_process->times_to_run == 0 && current_process->state == READY) {
                current_process->times_to_run = current_process->priority;
            }
		}

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
    
    PCBT *current_process = &(scheduler->processes[scheduler->index_rr]);
    
    if (current_process->state == DEAD) {
        scheduler->index_rr = (scheduler->index_rr + 1) % MAX_PROCESS;
        return stack_pointer;  
    }
    
    if (scheduler->quantum_remaining <= QUANTUM) {
        current_process->stack_pointer = stack_pointer;
    }
    
    current_process = update_quantum(stack_pointer);
    scheduler->current_pid = current_process->pid;
    
    return current_process->stack_pointer;
}

void list_processes_state() {
	SchedulerInfo scheduler = get_scheduler();
	drawWord("STAT - T: Blocked - S: Seady  - R: Running - Z: Zombie - <: Top priority - N: Lowest priority - +: Background - s: Session leader");
	commandEnter();
	commandEnter();
	drawWord("PID        STAT          RSP           RBP         COMMAND");
	commandEnter();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if(scheduler->processes[i].state != DEAD){
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
}

uint64_t kill_process(unsigned int pid) {
	SchedulerInfo scheduler = get_scheduler();
	if(pid == INIT_PID){
		drawWord("ERROR: You can not kill the Init process");
		return 0;
	}
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].pid == pid && scheduler->processes[i].state != ZOMBIE  && scheduler->processes[i].state != DEAD) {
			scheduler->processes[i].state = ZOMBIE; // Father process is in charge of changing process to DEAD with wait_children
			PCBT * parent = find_process(scheduler->processes[i].ppid);
			if(parent->waiting_pid == pid){
				unblock_process(parent->pid);
			}
			scheduler->amount_processes--;
			free_memory(scheduler->processes[i].stack_base);
			free_memory(scheduler->processes[i].argv);
			for(int j = 0; j < MAX_PROCESS; j++){
				if(scheduler->processes[j].ppid == pid && scheduler->processes[j].state != DEAD){
					scheduler->processes[j].ppid = INIT_PID;
					if(scheduler->processes[j].state == ZOMBIE){
						wait_children(scheduler->processes[j].pid);
					}
				}
			}
			if(scheduler->current_pid == pid){
				yield();
			}
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

PCBT * find_process(unsigned int pid){
	SchedulerInfo scheduler = get_scheduler();
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (scheduler->processes[i].pid == pid) {
			return &(scheduler->processes[i]);
		}
	}
	return NULL;
}

int get_current_file_descriptor_read(){
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = &(scheduler->processes[scheduler->index_rr]);
	return process->fds[STDIN];
}

int get_current_file_descriptor_write(){
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = &(scheduler->processes[scheduler->index_rr]);
	return process->fds[STDOUT];
}

int get_current_file_descriptor_error(){
	SchedulerInfo scheduler = get_scheduler();
	PCBT *process = &(scheduler->processes[scheduler->index_rr]);
	return process->fds[STDERR];
}