#include <stdint.h>
#include <stdio.h>
#include "syscall.h"
#include "test_util.h"
#include <utils.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 1  // TODO: Change as required
#define MEDIUM 2  // TODO: Change as required
#define HIGHEST 3 // TODO: Change as required


int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void idle_process() {
	while (1) {
		_hlt();
	}
}

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = create_process("idle", i, 0, LOWEST, RUNNING, FOREGROUND, NULL, 0, (main_function) idle_process);

  bussy_wait(WAIT);
  drawWord("CHANGING PRIORITIES...");
  enter(); 

  for (i = 0; i < TOTAL_PROCESSES; i++)
    update_priority(pids[i], prio[i]);

  bussy_wait(WAIT);
  drawWord("BLOCKING...");
  enter();

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block_process(pids[i]);

  drawWord("CHANGING PRIORITIES WHILE BLOCKED...");
  enter();

  for (i = 0; i < TOTAL_PROCESSES; i++)
    update_priority(pids[i], MEDIUM);

  drawWord("UNBLOCKING...");
  enter();

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblock_process(pids[i]);

  bussy_wait(WAIT);
  drawWord("KILLING...");
  enter();

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill_process(pids[i]);

    drawWord("FINISHED");
}