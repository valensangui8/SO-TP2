#ifndef _LIB_H
#define _LIB_H

#include <stdint.h>

void *my_memset(void *destination, int32_t character, uint64_t length);
void *my_memcpy(void *destination, const void *source, uint64_t length);
unsigned int my_strlen(const char *s);
void my_strncpy(char *dest, const char *src, unsigned int n);
unsigned int log(uint64_t n, int base);
void my_strcpy(char *dest, const char *src);
void my_strcat(char *dest, const char *src);

char *cpuVendor(char *result);
void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);
uint64_t get_key();
void enter();
void timer_tick();

#endif