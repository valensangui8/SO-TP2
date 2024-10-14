#include <stdio.h>
#include "syscall.h"
#include "test_util.h"
#include <utils.h>

// enum State { RUNNING,
//              BLOCKED,
//              KILLED };

typedef struct P_rq {
  int32_t pid;
  PCBState state;
} p_rq;

void idle_process2() {
	while (1) {
		_hlt();
	}
}

int64_t test_processes(uint64_t argc, char *argv[]) {
    
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};

  if (argc != 1)
    return -1;

  if ((max_processes = satoi(argv[0])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = create_process("idle", rq, 0, PRIORITY1, RUNNING, FOREGROUND, NULL, 0, (main_function) idle_process2);

      if (p_rqs[rq].pid == -1) {
        drawWord("test_processes: ERROR creating process");
        enter();
        return -1;
      } 

        p_rqs[rq].state = READY;
        alive++;
      
         drawWord("Estado del proceso creado: ");
    drawInt(p_rqs[rq].pid);
    drawWord(" - Estado: ");
    drawInt(p_rqs[rq].state);
    enter();
    }
    drawWord("test_processes: All processes created");
    enter();
    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED || p_rqs[rq].state == READY) {
              if (kill_process(p_rqs[rq].pid) == 0) {
                enter();
                drawWord("test_processes: ERROR killing process: ");
                drawInt(p_rqs[rq].pid);
                enter();
                return -1;
              }
              p_rqs[rq].state = DEAD;
              drawWord("test_processes: Process killed ");
              drawInt(p_rqs[rq].pid);
              enter();
              alive--;
            }
            break;

          case 1:

          drawWord("Intentando bloquear proceso: ");
            drawInt(p_rqs[rq].pid);
            enter();

            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == READY) {
              if (block_process(p_rqs[rq].pid) == 0) {
                drawWord("test_processes: ERROR blocking process");
                enter();
                return -1;
              }
              p_rqs[rq].state = BLOCKED;
                drawWord("test_processes: Proceso bloqueado ");
                drawInt(p_rqs[rq].pid);
                enter();
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (unblock_process(p_rqs[rq].pid) == 0) {
            drawWord("test_processes: ERROR unblocking process ");
            drawInt(p_rqs[rq].pid);
            enter();
            return -1;
          }
          p_rqs[rq].state = RUNNING;
            drawWord("test_processes: Process unblocked ");
            drawInt(p_rqs[rq].pid);
            enter();
        }
    }
    enter();
    drawWord("test_processes: All processes finished");
  }
  drawWord("test_processes: THE END ");
}