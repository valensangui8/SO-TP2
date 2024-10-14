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
	}
}

void timo(){
	drawWord("TImo puto");
	commandEnter();
	yield();
}



int main() {
	load_idt();

	initialize();

	create_process("Shell", 0, 0, 4, 1, 1, NULL, 0, (main_function) sampleCodeModuleAddress);

	// timer_tick();
	

	create_process("Timo", 1, 0, 1, 1, 1, NULL, 0, (main_function) timo);

	//((EntryPoint) sampleCodeModuleAddress)();

	

	while (1)
		;
	return 0;
}
