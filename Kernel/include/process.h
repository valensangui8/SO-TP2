#ifndef _PROCESS_H
#define _PROCESS_H

#include <scheduler.h>
#include <stdint.h>
#include <utils.h>


#define IDLE_PID 0

void init_procces(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, PCBState state, void *stack_pointer, char foreground, char **argv, int argc);
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
