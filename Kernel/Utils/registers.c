#include <register.h>



int flag_screenShot = 0;
void printRegisters(registerStructT * registers){
	if (flag_screenShot == 0){
		drawWord("Error - No hay screenshots de registros");
		return;
	}
	drawWord("RIP = 0x");
	drawRegisters(registers->rip);
	drawWord("RAX = 0x");
	drawRegisters(registers->rax);
	drawWord("RBX = 0x");
	drawRegisters(registers->rbx);
	drawWord("RCX = 0x");
	drawRegisters(registers->rcx);
	drawWord("RDX = 0x");
	drawRegisters(registers->rdx);
	drawWord("RSP = 0x");
	drawRegisters(registers->rsp);
	drawWord("RBP = 0x");
	drawRegisters(registers->rbp);
	drawWord("RSI = 0x");
	drawRegisters(registers->rsi);
	drawWord("RDI = 0x");
	drawRegisters(registers->rdi);
	drawWord("R8  = 0x");
	drawRegisters(registers->r8);
	drawWord("R9  = 0x");
	drawRegisters(registers->r9);
	drawWord("R10 = 0x");
	drawRegisters(registers->r10);
	drawWord("R11 = 0x");
	drawRegisters(registers->r11);
	drawWord("R12 = 0x");
	drawRegisters(registers->r12);
	drawWord("R13 = 0x");
	drawRegisters(registers->r13);
	drawWord("R14 = 0x");
	drawRegisters(registers->r14);
	drawWord("R15 = 0x");
	drawRegisters(registers->r15);
	drawWord("ss = 0x");
	drawRegisters(registers->ss);
	drawWord("cs = 0x");
	drawRegisters(registers->cs);
	drawWord("rflags = 0x");
	drawRegisters(registers->rflags);
}
