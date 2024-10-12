#ifndef _IDTMANAGER_H
#define _IDTMANAGER_H

#include <keyboardDriver.h>
#include <register.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <videoDriver.h>

void idtManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax);

#endif
