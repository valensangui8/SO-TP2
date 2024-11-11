// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <register.h>

int flag_screenShot = 0;
void print_registers(registerStructT *registers) {
	if (flag_screenShot == 0) {
		draw_word("Error - No hay screenshots de registros");
		return;
	}
	draw_word("RIP = 0x");
	draw_registers(registers->rip);
	draw_word("RAX = 0x");
	draw_registers(registers->rax);
	draw_word("RBX = 0x");
	draw_registers(registers->rbx);
	draw_word("RCX = 0x");
	draw_registers(registers->rcx);
	draw_word("RDX = 0x");
	draw_registers(registers->rdx);
	draw_word("RSP = 0x");
	draw_registers(registers->rsp);
	draw_word("RBP = 0x");
	draw_registers(registers->rbp);
	draw_word("RSI = 0x");
	draw_registers(registers->rsi);
	draw_word("RDI = 0x");
	draw_registers(registers->rdi);
	draw_word("R8  = 0x");
	draw_registers(registers->r8);
	draw_word("R9  = 0x");
	draw_registers(registers->r9);
	draw_word("R10 = 0x");
	draw_registers(registers->r10);
	draw_word("R11 = 0x");
	draw_registers(registers->r11);
	draw_word("R12 = 0x");
	draw_registers(registers->r12);
	draw_word("R13 = 0x");
	draw_registers(registers->r13);
	draw_word("R14 = 0x");
	draw_registers(registers->r14);
	draw_word("R15 = 0x");
	draw_registers(registers->r15);
	draw_word("ss = 0x");
	draw_registers(registers->ss);
	draw_word("cs = 0x");
	draw_registers(registers->cs);
	draw_word("rflags = 0x");
	draw_registers(registers->rflags);
}
