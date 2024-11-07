#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include <commands.h>
#include <stddef.h>
#include <Phylo.h>
#include <myStdio.h>

#define COMMANDS 25
#define FOREGROUND 1
#define BACKGROUND !FOREGROUND


typedef enum {VOID = 0, PROCESS_NO_PARAMS, PROCESS_PARAMS, FUNC_PARAMS} Type_t;

// shell manager
void initialize_shell(char *command, int argc, char **argv, char *command2, int argc2, char **argv2, int background);

// checks if the command is valid
int readCommand(char * command);

// executes the command
void executeCommand(int index, char * flag, char * command, int argc, char **argv, int16_t fds[], int *pid);
#endif



