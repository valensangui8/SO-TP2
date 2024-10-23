#ifndef _IDTMANAGER_H
#define _IDTMANAGER_H

#include <keyboardDriver.h>
#include <process.h>
#include <register.h>
#include <scheduler.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <utils.h>
#include <videoDriver.h>
#include <sound.h>

uint64_t idtManager(uint64_t rax, uint64_t *otherRegisters);

#endif
