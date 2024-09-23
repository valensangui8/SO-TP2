#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#include <stdint.h>
#include <register.h>
#include <buf_kb.h>
#include <lib.h>
#include <interrupts.h>

#define LEFT_SHIFT_NBR 42
#define RIGHT_SHIFT_NBR 54
#define RELEASED 128
#define LEFT_SHIFT_RELEASED 170
#define RIGHT_SHIFT_RELEASED 182

void keyboard_handler(); // function that handles the keyboard
uint8_t getKeyMapping(uint64_t number); // function that returns the key mapping 
void readChar(uint8_t * buf, uint32_t count, uint32_t * size); // function that reads the characters
extern uint64_t get_key(); // function that returns the key pressed value
extern void saveState(); // function that saves the state of the registers
#endif