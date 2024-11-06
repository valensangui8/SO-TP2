#ifndef _KEYBOARDDRIVER_H
#define _KEYBOARDDRIVER_H

#include <buf_kb.h>
#include <interrupts.h>
#include <lib.h>
#include <register.h>
#include <stdint.h>
#include <scheduler.h>

#define LEFT_SHIFT_NBR 42
#define RIGHT_SHIFT_NBR 54
#define RELEASED 128
#define LEFT_SHIFT_RELEASED 170
#define RIGHT_SHIFT_RELEASED 182
#define CTRL_PRESSED 29
#define CTRL_RELEASED 157
#define C_KEY 46
#define D_KEY 32

void keyboard_handler();									 // function that handles the keyboard
char getKeyMapping(uint64_t number);						 // function that returns the key mapping
void readChar(char *buf, uint32_t count, uint32_t *size); // function that reads the characters
extern uint64_t get_key();									 // function that returns the key pressed value
extern void saveState();									 // function that saves the state of the registers
#endif