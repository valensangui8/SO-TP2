#include <memoryManagerADT.h>
#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <utils.h>
#include <videoDriver.h>
#include <interrupts.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *) 0x400000;
static void *const sampleDataModuleAddress = (void *) 0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize) {
	my_memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
	return (void *) ((uint64_t) &endOfKernel + PageSize * 8 // The size of the stack itself, 32KiB
					 - sizeof(uint64_t)						// Begin at the top of the stack
	);
}
void *retUserland() {
	return sampleCodeModuleAddress;
}

void *initializeKernelBinary() {
	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress};

	uint64_t userspace_mem = loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	MemoryManagerADT mm = init_memory_manager(MM_SIZE, sampleDataModuleAddress + userspace_mem);
	create_scheduler();

	return getStackBase();
}

void idle() {
	while (1) {
		_hlt();
		yield();
	}
}

//funcion para imprimir los registros del stack
void print_stack_registers(PCBT *process) {
    void **stack_pointer = (void **)process->stack_pointer;

    // Imprimir los registros en el orden en que fueron empujados al stack
    drawWord("R15: "); drawHex((uint64_t)*(stack_pointer + 0)); commandEnter();
    drawWord("R14: "); drawHex((uint64_t)*(stack_pointer + 1)); commandEnter();
    drawWord("R13: "); drawHex((uint64_t)*(stack_pointer + 2)); commandEnter();
    drawWord("R12: "); drawHex((uint64_t)*(stack_pointer + 3)); commandEnter();
    drawWord("R11: "); drawHex((uint64_t)*(stack_pointer + 4)); commandEnter();
    drawWord("R10: "); drawHex((uint64_t)*(stack_pointer + 5)); commandEnter();
    drawWord("R9:  "); drawHex((uint64_t)*(stack_pointer + 6)); commandEnter();
    drawWord("R8:  "); drawHex((uint64_t)*(stack_pointer + 7)); commandEnter();
    drawWord("RSI: "); drawHex((uint64_t)*(stack_pointer + 8)); commandEnter();
    drawWord("RDI: "); drawHex((uint64_t)*(stack_pointer + 9)); commandEnter();
    drawWord("RBP: "); drawHex((uint64_t)*(stack_pointer + 10)); commandEnter();
    drawWord("RDX: "); drawHex((uint64_t)*(stack_pointer + 11)); commandEnter();
    drawWord("RCX: "); drawHex((uint64_t)*(stack_pointer + 12)); commandEnter();
    drawWord("RBX: "); drawHex((uint64_t)*(stack_pointer + 13)); commandEnter();
    drawWord("RAX: "); drawHex((uint64_t)*(stack_pointer + 14)); commandEnter();

    // Los siguientes son los valores de control
    drawWord("RIP: "); drawHex((uint64_t)*(stack_pointer + 15)); commandEnter();
    drawWord("CS:  "); drawHex((uint64_t)*(stack_pointer + 16)); commandEnter();
    drawWord("RFLAGS: "); drawHex((uint64_t)*(stack_pointer + 17)); commandEnter();
    drawWord("RSP: "); drawHex((uint64_t)*(stack_pointer + 18)); commandEnter();
    drawWord("SS:  "); drawHex((uint64_t)*(stack_pointer + 19)); commandEnter();
}

int main() {
	
	load_idt();
	initialize();

	// Crea el proceso "Shell"
	create_process("init", 1, 1, 1, NULL, 0, idle);
	
	create_process("Shell", 1, 4, 1, NULL, 0, (main_function) sampleCodeModuleAddress);

	//create_process("Idle", 1, 1, 0, NULL, 0, idle);

	SchedulerInfo scheduler = get_scheduler();
	print_stack_registers(scheduler->round_robin[0]);
	// print_stack_registers(scheduler->round_robin[1]);
	

	while (1)
		;
	return 0;
}





