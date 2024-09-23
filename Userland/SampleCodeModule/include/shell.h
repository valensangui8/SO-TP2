#ifndef SHELL_H
#define SHELL_H

#define COMMANDS 10

// shell manager
void initialize_shell(char * command);

// checks if the command is valid
int readCommand(char * command);

// executes the command
void executeCommand(int indexCommand, char * flag,  char * command);

#endif



