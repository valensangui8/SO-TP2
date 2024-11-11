#ifndef MYSTDIO_H
#define MYSTDIO_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <syscalls.h>

#define EOF (-1)
#define DEV_NULL -1
#define STDIN 0
#define STDOUT 1
#define STDERR 2

char get_char();
int strcmp(const char *s1, const char *s2);
int strcpy(char *dest, char *src);
void printf(char *fmt, ...);
void puts(char *buffer);
void put_char(char c);

int int_to_string(int num, char *str);
int hex_to_string(uint64_t num, char *toPrint);
char *strtok(char *str, const char *delim);
char *my_itoa(int num);
int my_atoi(char *str);

#endif
