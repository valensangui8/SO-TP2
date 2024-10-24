#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>


extern void call_sys_drawWord(char *word);
extern void call_sys_read(unsigned char *buf, uint32_t count, uint32_t *size);
extern void call_sys_drawChar(char letter);
extern void call_sys_delete();
extern void call_sys_enter();
extern void call_sys_drawError(char *command);
extern void call_sys_commandEnter();
extern void call_sys_zoomIn();
extern void call_sys_zoomOut();
extern int call_sys_get_date(int rax);
extern void call_sys_clear();
extern void call_sys_getScale(int *scale);
extern void call_sys_drawWithColor(char *word, uint32_t hexColor);
extern void call_sys_drawRegisters();
extern void call_sys_draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color);
extern void call_sys_sleep(unsigned long s);
extern void call_sys_sound(uint32_t nFrequence, uint32_t time);
extern void call_sys_checkHeight(char *HeightPassed, int command);
extern void call_sys_draw_int(int number);

extern uint64_t call_sys_kill_process(unsigned int pid);
extern void call_sys_update_priority(unsigned int pid, unsigned int new_priority);
extern uint16_t call_sys_block_process(unsigned int pid);
extern uint16_t call_sys_unblock_process(unsigned int pid);
extern void call_sys_yield();
extern void call_sys_process_status(unsigned int pid);
extern uint64_t call_sys_create_process(char *name, unsigned int priority, char foreground, char *argv[], int argc, void *rip);
extern void call_sys_list_processes_state();


extern int64_t call_sys_get_pid();
extern int64_t call_sys_get_ppid();
extern int64_t call_sys_wait_children(unsigned int ppid);
extern void call_sys_halt();

extern void *call_sys_alloc_memory(uint64_t size);
extern void *call_sys_free_memory(void *ptr);


extern int64_t call_sys_sem_open(char *sem_id, uint64_t initialValue);
extern int64_t call_sys_sem_wait(char *sem_id);
extern int64_t call_sys_sem_post(char *sem_id);
extern int64_t call_sys_sem_close(char *sem_id);
extern uint16_t call_sys_set_pid_state(unsigned int pid, int state);

#endif