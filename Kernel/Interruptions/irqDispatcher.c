#include <time.h>
#include <stdint.h>

void int_00();
void int_01();

void irqDispatcher(uint64_t irq) {
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

