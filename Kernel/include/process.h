#ifndef _PROCESS_H
#define _PROCESS_H

#include <scheduler.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <utils.h>

#define SESSION_LEADER 1
#define STACK_SIZE 4096

void init_process(PCBT *process, char *name, uint16_t pid, uint16_t ppid, Priority priority, char foreground, char **argv, int argc, main_function rip);
uint8_t has_children(unsigned int pid);
int64_t wait_children(unsigned int ppid);
void yield();
void run_process(PCBT *process);
void update_process_stack(void *sp);
void process_status(unsigned int pid);
char *process_state(PCBT process);
unsigned int get_pid();
unsigned int get_ppid();

#endif
