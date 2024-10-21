#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include <commands.h>
#include <stddef.h>

#define COMMANDS 13

// shell manager
void initialize_shell(char *command, int argc, char **argv);

// checks if the command is valid
int readCommand(char * command);

// executes the command
void executeCommand(int indexCommand, char * flag,  char * command, int argc, char **args);

#endif



