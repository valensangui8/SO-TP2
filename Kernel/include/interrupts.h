/*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <idtLoader.h>
#include <register.h>

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);
void _irq80Handler(void);

void _exception0Handler(void);
void _exception6Handler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void pic_master_mask(uint8_t mask);

void pic_slave_mask(uint8_t mask);

void save_state(void);
void print_reg_asm(void);

void halt_cpu(void);

void _yield();
void * _initialize_stack_frame(void *rip, void *function, void *stack, void *argv);

#endif /* _INTERRUPTS_H_ */
