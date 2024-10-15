#ifndef _PROCESS_H
#define _PROCESS_H

#include <scheduler.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <utils.h>

#define IDLE_PID 0
#define SESSION_LEADER 1

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, char foreground, char **argv, int argc, main_function rip);
uint8_t has_children(unsigned int pid);
int64_t wait_children(unsigned int ppid);
uint8_t kill_process(unsigned int pid);
void yield();
void run_process(PCBT *process);
uint16_t unblock_process(unsigned int pid);
uint16_t block_process(unsigned int pid);
void update_priority(unsigned int pid, Priority new_priority);
void update_process_stack(void *sp);
void process_status(unsigned int pid);
char *process_state(PCBT process);

#endif
