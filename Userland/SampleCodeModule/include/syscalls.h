
#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

extern void call_sys_drawWord(char *word);
extern void call_sys_read(unsigned char * buf, uint32_t count, uint32_t * size);
extern void call_sys_drawChar(char letter);
extern void call_sys_delete();
extern void call_sys_enter();
extern void call_sys_drawError(char * command);
extern void call_sys_commandEnter();
extern void call_sys_zoomIn();
extern void call_sys_zoomOut();
extern int call_sys_get_date(int rax);
extern void call_sys_clear();
extern void call_sys_getScale(int * scale);
extern void call_sys_drawWithColor(char * word, uint32_t hexColor);
extern void call_sys_drawRegisters();
extern void call_sys_draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color);
extern void call_sys_sleep(unsigned long s);
extern void call_sys_sound(uint32_t nFrequence, uint32_t time);
extern void call_sys_checkHeight(char * HeightPassed, int command);


#endif