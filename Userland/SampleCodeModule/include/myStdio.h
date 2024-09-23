#ifndef MYSTDIO_H
#define MYSTDIO_H

//hacer funciones en c para trabajar en los main

char getChar();
int strcmp(const char * s1, const char * s2);
int strcpy(char * dest, char * src);
void printf(char * fmt, ...);
void puts(char * buffer);

int intToString(int num, char *str);


#endif
