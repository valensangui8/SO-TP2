#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <process.h>
#include <utils.h>

#define QUANTUM 1
#define RR_INDEX(i) (((i) + 1) % (MAX_PROCESS * PRIORITY4))
#define INIT_PID 1

#define PID_WIDTH 6
#define STAT_WIDTH 8
#define RSP_WIDTH 10
#define RBP_WIDTH 10
#define COMMAND_WIDTH 20

SchedulerInfo create_scheduler();
uint8_t set_state(uint8_t new_state);
SchedulerInfo get_scheduler();
uint8_t get_state();
void *scheduler(void *stack_pointer);
PCBT *update_quantum(void *stack_pointer);
PCBT *find_process(unsigned int pid);

uint64_t create_process(char *name, Priority priority, char *argv[], int argc, main_function rip, const int16_t fds[]);
void list_processes_state(int *pids, char states[][10], uint64_t *rsps, uint64_t *rbps, char commands[][30], int *process_count);
uint64_t kill_process(unsigned int pid);
void kill_foreground_process();
uint16_t unblock_process(unsigned int pid);
uint16_t block_process(unsigned int pid);
uint64_t update_priority(unsigned int pid, Priority new_priority);
PCBT *find_process(unsigned int pid);

int get_current_file_descriptor_read();
int get_current_file_descriptor_write();
int get_current_file_descriptor_error();

int foreground();

#endif
