#ifndef _LIB_H
#define _LIB_H

#include <stdint.h>

#define EOF -1

void *my_memset(void *destination, int32_t character, uint64_t length);
void *my_memcpy(void *destination, const void *source, uint64_t length);
unsigned int my_strlen(const char *s);
void my_strncpy(char *dest, const char *src, unsigned int n);
unsigned int log(uint64_t n, int base);
void my_strcpy(char *dest, const char *src);
void my_strcat(char *dest, const char *src);
uint64_t my_strcmp(const char *s1, const char *s2);
uint8_t log2(uint64_t argument);
uint64_t pow2(uint64_t argument);
char *my_itoa(int num);
char *hexToChar(uint64_t num);


char *cpu_vendor(char *result);
void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);
uint64_t get_key();
void enter();
void timer_tick();
void acquire(uint8_t * lock);
void release(uint8_t * lock);

#endif