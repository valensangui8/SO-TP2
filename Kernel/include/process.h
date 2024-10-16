#ifndef _PROCESS_H
#define _PROCESS_H

#include <scheduler.h>
#include <stack.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <utils.h>

#define IDLE_PID 0

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, char foreground, char **argv, int argc, main_function rip);
uint8_t has_children(unsigned int pid);
void wait_children(unsigned int ppid);
uint8_t kill_process(unsigned int pid);
void yield();
void run_process(PCBT *process);
uint16_t unblock_process(unsigned int pid);
uint16_t block_process(unsigned int pid);
void update_priority(unsigned int pid, Priority new_priority);
void update_process_stack(void *sp);
void process_status(int pid);
char *process_state(PCBState state, Priority priority, char foreground);

#endif
