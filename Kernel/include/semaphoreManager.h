#ifndef _SEMAPHOREMANAGER_H
#define _SEMAPHOREMANAGER_H

#include <stdint.h>
#include <memoryManagerADT.h>
#include <lib.h>
#include <linkedlist.h>
#include <utils.h>

#define MAX_SEMAPHORE 20

typedef struct SemaphoresCDT * SemaphoresADT;
typedef struct MySem * MySem_t;

SemaphoresADT create_semaphoreADT();
SemaphoresADT get_semaphoresADT();

MySem_t get_semaphore(char *sem_id);
int add_semaphore(MySem_t sem);

int64_t sem_open(char *sem_id, uint64_t initial_value);
int64_t sem_wait(char *sem_id);
int64_t sem_post(char *sem_id);
int64_t sem_close(char * sem_id);

#endif