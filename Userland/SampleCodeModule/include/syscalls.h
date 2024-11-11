#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

extern void call_sys_draw_word(char *word);
extern void call_sys_error(char *word, int fd);
extern void call_sys_read(char *buf, uint32_t count, uint32_t *size);
extern void call_sys_draw_char(char letter);
extern void call_sys_delete();
extern void call_sys_enter();
extern void call_sys_draw_error(char *command);
extern void call_sys_command_enter();
extern void call_sys_zoom_in();
extern void call_sys_zoom_out();
extern int call_sys_get_date(int rax);
extern void call_sys_clear();
extern void call_sys_get_scale(int *scale);
extern void call_sys_draw_with_color(char *word, uint32_t hexColor);
extern void call_sys_draw_registers();
extern void call_sys_draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color);
extern void call_sys_sleep(unsigned long s);
extern void call_sys_sound(uint32_t nFrequence, uint32_t time);
extern void call_sys_check_height(char *HeightPassed, int command);
extern void call_sys_draw_int(int number);

extern uint64_t call_sys_kill_process(unsigned int pid);
extern uint64_t call_sys_update_priority(unsigned int pid, unsigned int new_priority);
extern uint16_t call_sys_block_process(unsigned int pid);
extern uint16_t call_sys_unblock_process(unsigned int pid);
extern void call_sys_yield();
extern void call_sys_process_status(unsigned int pid);
extern uint64_t call_sys_create_process(char *name, unsigned int priority, char *argv[], int argc, void *rip, int16_t fds[]);
extern void call_sys_list_processes_state(int *pids, char states[][10], uint64_t *rsps, uint64_t *rbps, char commands[][30], int *process_count);

extern int64_t call_sys_get_pid();
extern int64_t call_sys_get_ppid();
extern int64_t call_sys_wait_children(unsigned int pid);
extern void call_sys_halt();

extern void *call_sys_alloc_memory(uint64_t size);
extern void *call_sys_free_memory(void *ptr);

extern int64_t call_sys_sem_open(char *sem_id, uint64_t initialValue);
extern int64_t call_sys_sem_wait(char *sem_id);
extern int64_t call_sys_sem_post(char *sem_id);
extern int64_t call_sys_sem_close(char *sem_id);

extern int16_t call_sys_get_pipe_fd();
extern int16_t call_sys_open_pipe(int id, char mode, int pid);
extern int16_t call_sys_close_pipe(uint16_t fd, int pid);
extern int16_t call_sys_write_pipe(uint16_t fd, char *buffer, uint16_t *count);
extern int16_t call_sys_read_pipe(uint16_t fd, char *buffer, uint16_t *count);

extern void call_sys_get_memory_info(char *type, uint64_t *free, uint64_t *allocated, uint64_t *total);

extern int call_sys_foreground();

#endif