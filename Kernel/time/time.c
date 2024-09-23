#include <time.h>

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

unsigned long  ms_elapsed() {
	return ticks * 55;
}

void sleep(unsigned long ms) {
	unsigned long  intial_time = ms_elapsed();
	unsigned long  currentTime = intial_time;
    while( currentTime - intial_time <= ms) {
		currentTime = ms_elapsed();
	    _hlt();
    }
}
