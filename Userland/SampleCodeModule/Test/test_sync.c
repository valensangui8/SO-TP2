#include <stdint.h>
#include <stdio.h>
#include <syscalls.h>
#include "test_util.h"

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


  if (argc != 3){
    return -1;
  }

  if ((n = satoi(argv[1])) <= 0){
    return -1;
  }
  
  if ((inc = satoi(argv[2])) == 0){
    return -1;
  }
  if ((use_sem = satoi(argv[3])) < 0){
    return -1;
  }

  if (use_sem)
    if (!call_sys_sem_open(SEM_ID, 1)) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem){
      call_sys_sem_wait(SEM_ID);
    }
    slowInc(&global, inc);
    if (use_sem){
      call_sys_sem_post(SEM_ID);
    }
  }

  if (use_sem){
    call_sys_sem_close(SEM_ID);
  }


  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  
  

  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3){
    return -1;
  }
  

  char *argvDec[] = {argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {argv[1], "1", argv[2], NULL};

  global = 0;

  uint64_t i;
  
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    
    pids[i] = call_sys_create_process("my_process_inc", 1, 0, argvDec, 3, &my_process_inc);
   
    pids[i + TOTAL_PAIR_PROCESSES] = call_sys_create_process("my_process_inc", 1, 0, argvInc, 3, &my_process_inc);
   
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    call_sys_wait_children(pids[i]);
    call_sys_wait_children(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printf("Final value: %d", global);

}