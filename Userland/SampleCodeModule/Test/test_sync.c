// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"
#include <stdint.h>
#include <stdio.h>
#include <syscalls.h>

#define SEM_ID "10"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
	uint64_t aux = *p;
	call_sys_yield(); // This makes the race condition highly probable
	aux += inc;
	*p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
	uint64_t n;
	int8_t inc;
	int8_t use_sem;

	if (argc != 3) {
		return -1;
	}

	if ((n = satoi(argv[0])) <= 0) {
		return -1;
	}

	if ((inc = satoi(argv[1])) == 0) {
		return -1;
	}

	if ((use_sem = satoi(argv[2])) < 0) {
		return -1;
	}

	if (use_sem) {
		if (call_sys_sem_open(SEM_ID, 1) == -1) {
			printf("test_sync: ERROR opening semaphore\n");
			return -1;
		}
	}

	uint64_t i;
	for (i = 0; i < n; i++) {
		if (use_sem) {
			// call_sys_draw_word(" START WAIT ");
			call_sys_sem_wait(SEM_ID);
			// call_sys_draw_word(" - STOP WAIT ");
		}
		slowInc(&global, inc);
		if (use_sem) {
			call_sys_sem_post(SEM_ID);
			// call_sys_draw_word(" POST DONE ");
		}
	}

	if (use_sem) {
		call_sys_sem_close(SEM_ID);
	}
	return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
	uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

	if (argc != 3) {
		return -1;
	}

	char *argvDec[] = {argv[1], "-1", argv[2], NULL};
	char *argvInc[] = {argv[1], "1", argv[2], NULL};

	global = 0;

	uint64_t i;

	int16_t fds[3] = {0, 1, 1};

	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		pids[i] = call_sys_create_process("my_process_inc", 1, argvDec, 3, &my_process_inc, fds);

		pids[i + TOTAL_PAIR_PROCESSES] = call_sys_create_process("my_process_inc", 1, argvInc, 3, &my_process_inc, fds);
	}
	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		call_sys_wait_children(pids[i]);
		call_sys_wait_children(pids[i + TOTAL_PAIR_PROCESSES]);
	}

	printf("Final value: %d", global);
	return 0;
}
