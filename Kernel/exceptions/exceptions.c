#include <register.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_ID 6


static void zero_division();
static void invalid_op();

void exception_dispatcher(int exception, registerStructT * registers) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if(exception == INVALID_OP_ID ){
		invalid_op();
	}
	print_registers(registers);
	enter();
}

void zero_division() {
	draw_word("ERROR: division by zero");
	command_enter();
	flag_screenShot = 1;
}

void invalid_op() {
	draw_word("ERROR: invalid operation");
	command_enter();
	flag_screenShot = 1;
}
