// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>
#include <stdint.h>
#include <time.h>

void int_00();
void int_01();

void irq_dispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_00();
			break;
		case 1:
			int_01();
			break;
	}
	return;
}

void int_00() {
	timer_handler();
}

void int_01() {
	keyboard_handler();
}
