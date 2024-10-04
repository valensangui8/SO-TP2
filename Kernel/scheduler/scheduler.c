#include "scheduler.h"

typedef struct SchedulerCDT {
    PCBT * processes[MAX_PROCESS];
    PCBT * round_robin[MAX_PROCESS * PRIORITY4];
    uint16_t current_pid;
    uint16_t index_p;
    uint16_t index_rr;
    int8_t quantum_remaining;

} SchedulerCDT;

SchedulerADT create_scheduler() {
	SchedulerADT scheduler = (SchedulerADT) SCHEDULER_ADDRESS;
	for(int i=0; i< MAX_PROCESS;i++){
		scheduler->processes[i] = NULL; // No process when initialized
	}
    
    for(int i=0; i< MAX_PROCESS * PRIORITY4;i++){
        scheduler->round_robin[i] = NULL; // No process when initialized
    }
    
    scheduler->current_pid = 0;
    scheduler->index_p = 0;
    scheduler->index_rr = 0;
    scheduler->quantum_remaining = QUANTUM;
	
	return scheduler;
}

SchedulerADT get_scheduler(){
	return (SchedulerADT) SCHEDULER_ADDRESS;
}

int8_t set_status(uint8_t new_status) {
    SchedulerADT scheduler = get_scheduler();
    PCBT * process = scheduler->round_robin[scheduler->index_rr];
    if (process == NULL) {
        return -1;
    }
    PCBStatus old_status = process->status;
	if(new_status == ZOMBIE || new_status == RUNNING || old_status == ZOMBIE){
		return -1;
	}
	process->status = new_status;
	if(new_status == old_status){
		return old_status;
	}
    
    return new_status;
}

int8_t get_status() {
    SchedulerADT scheduler = get_scheduler();
    PCBT * process = scheduler->round_robin[scheduler->index_rr];
    if (process == NULL) {
        return -1;
    }
    return process->status;
}


