#ifndef _PROCESS_H
#define _PROCESS_H

#include <utils.h>
#include <stack.h>
#include <scheduler.h>

#define IDLE_PID 0

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, char foreground, char **argv, int argc, uint64_t rip);
uint8_t has_children(unsigned int pid);
void wait_children(unsigned int ppid);
uint8_t kill_process(unsigned int pid);
void yield();
void run_process(PCBT *process);
void unblock_process(unsigned int pid);
void block_process(unsigned int pid);
void update_priority(unsigned int pid, Priority new_priority);
void update_process_stack(void *sp);

#endif
