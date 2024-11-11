// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>

extern void _hlt();

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

unsigned long ms_elapsed() {
	return ticks * 55;
}

void sleep(unsigned long ms) {
	unsigned long intial_time = ms_elapsed();
	unsigned long currentTime = intial_time;
	while (currentTime - intial_time <= ms) {
		currentTime = ms_elapsed();
		_hlt();
	}
}
