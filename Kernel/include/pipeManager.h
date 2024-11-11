#ifndef _PIPE_MANAGER_H
#define _PIPE_MANAGER_H

#include <stdint.h>
#include <utils.h>
#include <semaphoreManager.h>
#include <memoryManagerADT.h>
#include <lib.h>

#define PIPE_SIZE (1 << 12)

typedef struct PipeManagerCDT * PipeManagerADT;

PipeManagerADT create_pipe_manager();
int16_t get_pipe_fd();
int16_t open_pipe(int id, char mode, int pid);
int16_t close_pipe(uint16_t fd);
int32_t write_pipe(uint16_t fd, char *buffer, uint32_t *count);
int32_t read_pipe(uint16_t fd, char *buffer, uint32_t *count);
 

#endif