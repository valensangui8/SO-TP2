#ifndef IDTMANAGER_H
#define IDTMANAGER_H

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <register.h>
#include <time.h>

void idtManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax);

#endif
