#include <syscalls.h>
#include "test_util.h"
#include <stdint.h>
#include <stdio.h>

#define MINOR_WAIT 10000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10		  // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 10
#define LOWEST 1  // TODO: Change as required
#define MEDIUM 2  // TODO: Change as required
#define HIGHEST 3 // TODO: Change as required

#define FOREGROUND 1
#define BACKGROUND !FOREGROUND

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void idle_process() {
	while (1) {
		call_sys_halt();
	}
}

void test_prio() {
	int64_t pids[TOTAL_PROCESSES];
	uint64_t i;

	for (i = 0; i < TOTAL_PROCESSES; i++){
		pids[i] = call_sys_create_process("idle", LOWEST, FOREGROUND, NULL, 0, &idle_process);
	}

	
	bussy_wait(WAIT);
	call_sys_drawWord("CHANGING PRIORITIES...");
	call_sys_commandEnter();

	for (i = 0; i < TOTAL_PROCESSES; i++)
		call_sys_update_priority(pids[i], prio[i]);

	bussy_wait(WAIT);
	call_sys_drawWord("BLOCKING...");
	call_sys_commandEnter();

	for (i = 0; i < TOTAL_PROCESSES; i++)
		call_sys_block_process(pids[i]);

	call_sys_drawWord("CHANGING PRIORITIES WHILE BLOCKED...");
	call_sys_commandEnter();

	for (i = 0; i < TOTAL_PROCESSES; i++)
		call_sys_update_priority(pids[i], MEDIUM);

	call_sys_drawWord("UNBLOCKING...");
	call_sys_commandEnter();

	for (i = 0; i < TOTAL_PROCESSES; i++)
		call_sys_unblock_process(pids[i]);

	bussy_wait(WAIT);
	call_sys_drawWord("KILLING...");
	call_sys_commandEnter();

	for (i = 0; i < TOTAL_PROCESSES; i++){
		call_sys_kill_process(pids[i]);
	}

	call_sys_drawWord("FINISHED");
	call_sys_enter();
}