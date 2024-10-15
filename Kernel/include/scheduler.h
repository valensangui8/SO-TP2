#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <utils.h>

#define QUANTUM 2
#define RR_INDEX(i) ((i + 1) % (MAX_PROCESS * PRIORITY4))

#define PID_WIDTH 6
#define STAT_WIDTH 8
#define RSP_WIDTH 10
#define RBP_WIDTH 10
#define COMMAND_WIDTH 20

SchedulerInfo create_scheduler();
int8_t set_state(uint8_t new_state);
SchedulerInfo get_scheduler();
int8_t get_state();
uint16_t create_process(char *name, uint16_t ppid, Priority priority, char foreground, char *argv[], int argc, main_function rip);
unsigned int get_pid();
void* scheduler(void *stack_pointer);
PCBT *update_quantum(void *stack_pointer);
void list_processes_state();

#endif
