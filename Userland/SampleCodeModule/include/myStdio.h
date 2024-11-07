#ifndef MYSTDIO_H
#define MYSTDIO_H

#include <syscalls.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>


#define EOF (-1)

char getChar();
int strcmp(const char * s1, const char * s2);
int strcpy(char * dest, char * src);
void printf(char * fmt, ...);
void puts(char * buffer);
void putchar(char c);

int intToString(int num, char *str);
int hexToString(uint64_t num, char *toPrint);
char * strtok(char * str, const char * delim);
char *my_itoa(int num);
int my_atoi(char *str);



#endif
