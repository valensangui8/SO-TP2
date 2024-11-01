#include <idtManager.h>

static void sys_Read(uint8_t *buf, uint32_t count, uint32_t *size);
static void sys_DrawWord(char *word);
static void sys_DrawChar(char letter);
static void sys_delete();
static void sys_enter();
static void sys_drawError(char *command);
static void sys_commandEnter();
static void sys_zoomIn();
static void sys_zoomOut();
static void sys_clear();
static void sys_getScale(int *scale);
static void sys_drawWithColor(char *word, uint32_t hexColor);
static void sys_drawRegisters();
static void sys_draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color);
static void sys_sleep(unsigned long s);
static void sys_sound(uint32_t nFrequence, uint32_t time);
static void sys_checkHeight(char *HeightPassed, int indexCommand);
static void sys_draw_int(int number);

static uint64_t sys_kill_process(unsigned int pid);
static void sys_update_priority(unsigned int pid, Priority new_priority);
static uint16_t sys_block_process(unsigned int pid);
static uint16_t sys_unblock_process(unsigned int pid);
static void sys_yield();
static void sys_process_status(unsigned int pid);
static uint64_t sys_create_process(char *name, Priority priority, char foreground, char *argv[], int argc, main_function rip);
static void sys_list_processes_state();

static int64_t sys_get_pid();
static int64_t sys_get_ppid();
static int64_t sys_wait_children(unsigned int ppid);
static void sys_halt();

static void *sys_alloc_memory(uint64_t size);
static void sys_free_memory(void *ptr);

static int64_t sys_sem_open(char *sem_id, uint64_t initialValue);
static int64_t sys_sem_wait(char *sem_id);
static int64_t sys_sem_post(char *sem_id);
static int64_t sys_sem_close(char *sem_id);



uint64_t idtManager(uint64_t rax, uint64_t *otherRegisters) {
    uint64_t rdi, rsi, rdx, rcx, r8, r9;
    rdi = otherRegisters[0];
    rsi = otherRegisters[1];
    rdx = otherRegisters[2];
    rcx = otherRegisters[3];
    r8 = otherRegisters[4];
    r9 = otherRegisters[5];
	switch (rax) {
		case 0:															 // read
			sys_Read((uint8_t *) rdi, (uint32_t) rsi, (uint32_t *) rdx); // rdi = buffer ; rsi = size , rdx = count
			break;
		case 1:							// write
			sys_DrawWord((char *) rdi); // rdi = palabra
			break;
		case 2:
			sys_DrawChar((char) rdi); // rdi = letra
			break;
		case 3:
			sys_enter();
			break;
		case 4:
			sys_delete();
			break;
		case 5:
			sys_drawError((char *) rdi);
			break;
		case 6:
			sys_commandEnter();
			break;
		case 7:
			sys_zoomIn();
			break;
		case 8:
			sys_zoomOut();
			break;
		case 9:
			sys_clear();
			break;
		case 10:
			sys_getScale((int *) rdi);
			break;
		case 11:
			sys_drawWithColor((char *) rdi, (uint32_t) rsi);
			break;
		case 12:
			sys_drawRegisters();
			break;
		case 13:
			sys_draw((uint32_t) rdi, (uint32_t) rsi, (uint32_t) rdx, (uint32_t) rcx);
			break;
		case 14:
			sys_sleep((unsigned long) rdi);
			break;
		case 15:
			sys_sound((uint32_t) rdi, (uint32_t) rsi);
			break;
		case 16:
			sys_checkHeight((char *) rdi, (uint32_t) rsi);
			break;
		case 17:
			return sys_kill_process((unsigned int) rdi);
			break;
		case 18:
			sys_update_priority((unsigned int) rdi, (Priority) rsi);
			break;
		case 19:
			return sys_block_process((unsigned int) rdi);
			break;
		case 20:
			return sys_unblock_process((unsigned int) rdi);
			break;
		case 21:
			sys_yield();
			break;
		case 22:
			sys_process_status((unsigned int) rdi);
			break;
		case 23:
			return sys_create_process((char *) rdi, (Priority) rsi, (char) rdx, (char **) rcx, (int) r8, (main_function) r9);
		
		case 24:
			sys_list_processes_state();
			break;
		case 25:
			return sys_get_pid();
			break;
		case 26:
			return sys_get_ppid();
		case 27:
			return sys_wait_children((unsigned int) rdi);
			break;
		case 28:
			sys_draw_int((int) rdi);
			break;
		case 29:
			sys_halt();
			break;
		case 30:
			return (uint64_t) sys_alloc_memory((uint64_t) rdi);
		case 31:
			sys_free_memory((void *) rdi);
			break;
		case 32:
			return sys_sem_open((char *) rdi, (uint64_t) rsi);
			break;
		case 33:
			return sys_sem_wait((char *) rdi);
			break;
		case 34:
			return sys_sem_post((char *) rdi);
			break;
		case 35:
			return sys_sem_close((char *) rdi);
			break;
	}
	return 0;
}

void sys_Read(uint8_t *buf, uint32_t count, uint32_t *size) {
	readChar(buf, count, size);
}

void sys_DrawWord(char *word) {
	drawWord(word);
}

void sys_DrawChar(char letter) {
	drawLine(letter);
}

void sys_enter() {
	enter();
}

void sys_delete() {
	delete ();
}

void sys_drawError(char *command) {
	drawError(command);
}

void sys_commandEnter() {
	commandEnter();
}

void sys_zoomIn() {
	incScale();
}

void sys_zoomOut() {
	decScale();
}

void sys_clear() {
	clear();
}

void sys_getScale(int *scale) {
	*scale = getScale();
}

void sys_drawWithColor(char *word, uint32_t hexColor) {
	drawWithColor(word, hexColor);
}

void sys_drawRegisters() {
	printRegAsm();
}

void sys_draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color) {
	draw(x, y, size, color);
}

void sys_sleep(unsigned long s) {
	sleep(s);
}

void sys_sound(uint32_t nFrequence, uint32_t time) {
	start_sound(nFrequence, time);
}

void sys_checkHeight(char *HeightPassed, int command) {
	checkHeight(HeightPassed, command);
}

uint64_t sys_kill_process(unsigned int pid) {
	return kill_process(pid);
}

void sys_update_priority(unsigned int pid, Priority new_priority) {
	update_priority(pid, new_priority);
}

uint16_t sys_block_process(unsigned int pid) {
	return block_process(pid);
}

uint16_t sys_unblock_process(unsigned int pid) {
	return unblock_process(pid);
}

void sys_yield() {
	yield();
}

void sys_process_status(unsigned int pid) {
	process_status(pid);
}

uint64_t sys_create_process(char *name, Priority priority, int file_descriptors[], char *argv[], int argc, main_function rip) {
	uint64_t pid = create_process(name, priority, file_descriptors, argv, argc, rip);
	
	return pid;
}

void sys_list_processes_state() {
	list_processes_state();
}

int64_t sys_wait_children(unsigned int ppid) {
	return wait_children(ppid);
}

int64_t sys_get_pid() {
	return get_pid();
}

int64_t sys_get_ppid() {
	return get_ppid();
}

void sys_draw_int(int number) {
	drawInt(number);
}

void sys_halt() {
	_hlt();
}

void *sys_alloc_memory(uint64_t size){
	return alloc_memory(size);
}

void sys_free_memory(void *ptr){
	free_memory(ptr);
}

int64_t sys_sem_open(char *sem_id, uint64_t initialValue){
	return sem_open(sem_id, initialValue);
}

int64_t sys_sem_wait(char *sem_id){
	return sem_wait(sem_id);
}

int64_t sys_sem_post(char *sem_id){
	return sem_post(sem_id);
}

int64_t sys_sem_close(char *sem_id){
	return sem_close(sem_id);
}