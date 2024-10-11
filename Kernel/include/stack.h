#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define GPR_QTY 15 // son los registros que usamos en pushState y popState

typedef struct stack
{
	uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rbx;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rax;
    uint64_t rip;           
    uint64_t cs;            
    uint64_t rflags;       
    uint64_t rsp;            
    uint64_t ss;            
} stack;

uint64_t load_stack(uint64_t rip, uint64_t rsp);
#endif
