// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include "test_util.h"
#include <stdint.h>
#include <stdio.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 1000000000		  // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice
#define WAIT_STR "300000000"

#define TOTAL_PROCESSES 4
#define LOWEST 1  // TODO: Change as required
#define MEDIUM 2  // TODO: Change as required
#define MEDIUM_HIGH 3
#define HIGHEST 4 // TODO: Change as required

#define FOREGROUND 1
#define BACKGROUND !FOREGROUND

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM,MEDIUM_HIGH, HIGHEST};


void test_prio() {
	int64_t pids[TOTAL_PROCESSES];
	uint64_t i;
	char * argvAux[] = {WAIT_STR};
	int16_t fds[3] = {0, 1, 1};
	for (i = 0; i < TOTAL_PROCESSES; i++){
		pids[i] = call_sys_create_process("endeless_loop_print", LOWEST, argvAux, 1, &endless_loop_print, fds);
		
	}

	bussy_wait(WAIT);
	call_sys_draw_word("CHANGING PRIORITIES...");
	call_sys_command_enter();

	for (i = 0; i < TOTAL_PROCESSES; i++)
		call_sys_update_priority(pids[i], prio[i]);

	bussy_wait(WAIT);
	call_sys_draw_word("BLOCKING...");
	call_sys_command_enter();

	for (i = 0; i < TOTAL_PROCESSES; i++)
		call_sys_block_process(pids[i]);

	call_sys_draw_word("CHANGING PRIORITIES WHILE BLOCKED...");
	call_sys_command_enter();

	for (i = 0; i < TOTAL_PROCESSES; i++){
		call_sys_update_priority(pids[i], MEDIUM);
	}

	call_sys_draw_word("UNBLOCKING...");
	call_sys_command_enter();

	for (i = 0; i < TOTAL_PROCESSES; i++)
		call_sys_unblock_process(pids[i]);

	bussy_wait(WAIT);
	call_sys_draw_word("KILLING...");
	call_sys_command_enter();

	for (i = 0; i < TOTAL_PROCESSES; i++){
		call_sys_kill_process(pids[i]);
	}

	call_sys_draw_word("FINISHED");
	call_sys_command_enter();
	call_sys_kill_process(call_sys_get_pid());
}