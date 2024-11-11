#ifndef PHILO_H
#define PHILO_H

#include <stdint.h>
#include <myStdio.h>
#include <syscalls.h>
#include <stdlib.h>

#define MAX_PHILOSOPHERS 10
#define MIN_PHILOSOPHERS 5
#define ADD 'a'
#define REMOVE 'r'
#define QUIT 'q'
#define THINKING '.'
#define EATING 'E'
#define EATING_TIME 3
#define THINKING_TIME 1


// Process function for individual philosophers
void philosopher_process(int argc, char **argv);

// Table manager process function
void table_manager();

// Entry function to start the dining philosophers process
int philo(int16_t fds[]);

#endif // PHILO_H
