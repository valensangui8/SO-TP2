
#include <idtLoader.h>
#include <moduleLoader.h>
#include <time.h>
#include <utils.h>
#include <videoDriver.h>
#include <scheduler.h>
#include <semaphoreManager.h>
#include <pipeManager.h>

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

void *get_stack_base() {
	return (void *) ((uint64_t) &endOfKernel + PageSize * 8 // The size of the stack itself, 32KiB
					 - sizeof(uint64_t)						// Begin at the top of the stack
	);
}
void *ret_userland() {
	return sampleCodeModuleAddress;
}

void *initializeKernelBinary() {
	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress};

	uint64_t userspace_mem = loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	init_memory_manager(MM_SIZE, sampleDataModuleAddress + userspace_mem);
	create_scheduler();
	create_semaphoreADT();
	create_pipe_manager();

	return get_stack_base();
}

void idle() {
	while (1) {
		_hlt();
		//yield();
	}
}


int main() {
	_cli();
	load_idt();
	int16_t fd[] = {STDIN, STDOUT, STDERR};
	create_process("init", PRIORITY1, NULL, 0,(main_function) &idle, fd);
	create_process("Shell", PRIORITY4, NULL, 0, (main_function) sampleCodeModuleAddress, fd);

	start();
	sleep(500);
	clear();


	_sti();
	initialize();
	while(1);

	return 0;
}





