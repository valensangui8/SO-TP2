#ifndef PHILO_H
#define PHILO_H

#include <stdint.h>
#include <myStdio.h>
#include <syscalls.h>
#include <stdlib.h>

#define MAX_PHILOSOPHERS 10
#define MIN_PHILOSOPHERS 2
#define ADD 'a'
#define REMOVE 'r'
#define QUIT 'q'
#define THINKING '.'
#define EATING 'E'
#define HUNGRY 'H'
#define EATING_TIME = 1;

// Function to initialize the philosophers and start the table manager process
int initialize_philosophers();


// Process function for individual philosophers
void philosopher_process(int argc, char **argv);

// Function to print the current state of the table
void print_table_state();

// Table manager process function
void table_manager();

// Entry function to start the dining philosophers process
int philo(int16_t fds[]);

#endif // PHILO_H
