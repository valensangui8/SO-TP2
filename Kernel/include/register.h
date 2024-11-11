#ifndef _REGISTERS_H
#define _REGISTERS_H

#define REGISTERS 20

#include <stdint.h>
#include <videoDriver.h>
// structure of registers that will be saved in the snapshot and interrupts
typedef struct registerStructT {
	uint64_t rax, rbx, rcx, rdx;
	uint64_t rsi, rdi, rbp, rsp;
	uint64_t r8, r9, r10, r11;
	uint64_t r12, r13, r14, r15;
	uint64_t ss, cs, rflags, rip;
} registerStructT;

#include <interrupts.h>

extern registerStructT *registers;
extern int flag_screenShot;

void print_registers(registerStructT *registers);
#endif /* _REGISTERS_H */