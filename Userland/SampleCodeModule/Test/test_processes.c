// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"
#include <stdio.h>
#include <syscalls.h>

#define FOREGROUND 1
#define BACKGROUND !FOREGROUND

enum State { BLOCKED = 0,
			 READY,
			 RUNNING,
			 ZOMBIE,
			 DEAD };

enum PRIORITY { PRIORITY1 = 1,
				PRIORITY2,
				PRIORITY3,
				PRIORITY4 };

typedef struct P_rq {
	int32_t pid;
	enum State state;
} p_rq;

void idle_process2() {
	while (1) {
		call_sys_halt();
	}
}

int64_t test_processes(int argc, char **argv) {
	uint8_t rq;
	uint8_t alive = 0;
	uint8_t action;
	uint64_t max_processes;

	if (argc == 1) {
		return -1;
	}

	max_processes = satoi(argv[1]);

	if (max_processes <= 0) {
		return -1;
	}

	p_rq p_rqs[max_processes];
	int16_t fds[3] = {0, 1, 1};

	while (1) {
		// Create max_processes processes
		for (rq = 0; rq < max_processes; rq++) {
			p_rqs[rq].pid = call_sys_create_process("endless_loop", PRIORITY1, NULL, 0, &endless_loop, fds);

			if (p_rqs[rq].pid == -1) {
				call_sys_draw_word("test_processes: ERROR creating process");
				call_sys_command_enter();

				return -1;
			}

			p_rqs[rq].state = READY;
			alive++;
		}

		while (alive > 0) {
			for (rq = 0; rq < max_processes; rq++) {
				action = GetUniform(100) % 2;
				switch (action) {
					case 0:
						if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED || p_rqs[rq].state == READY) {
							if (call_sys_kill_process(p_rqs[rq].pid) == 0) {
								call_sys_draw_word("test_processes: ERROR Killing process");
								return -1;
							}
							p_rqs[rq].state = DEAD;
							call_sys_wait_children(p_rqs[rq].pid);

							alive--;
						}
						break;

					case 1:
						if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == READY) {
							if (call_sys_block_process(p_rqs[rq].pid) == 0) {
								call_sys_draw_word("test_processes: ERROR blocking process");
								call_sys_command_enter();

								return -1;
							}
							p_rqs[rq].state = BLOCKED;
							call_sys_yield();
						}
						break;
				}
			}

			// Randomly unblocks processes
			for (rq = 0; rq < max_processes; rq++)
				if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
					if (call_sys_unblock_process(p_rqs[rq].pid) == 0) {
						call_sys_draw_word("test_processes: ERROR unblocking process ");
						return -1;
					}
					p_rqs[rq].state = RUNNING;
				}
		}
	}
}