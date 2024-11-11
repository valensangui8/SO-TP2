#ifndef _idt_manager_H
#define _idt_manager_H

#include <keyboardDriver.h>
#include <pipeManager.h>
#include <process.h>
#include <register.h>
#include <scheduler.h>
#include <semaphoreManager.h>
#include <sound.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <utils.h>
#include <videoDriver.h>

uint64_t idt_manager(uint64_t rax, uint64_t *otherRegisters);

#endif
