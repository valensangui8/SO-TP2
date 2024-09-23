#include <register.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_ID 6


static void zero_division();

void exceptionDispatcher(int exception, registerStructT * registers) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if(exception == INVALID_OP_ID ){
		invalid_op();
	}
	printRegisters(registers);
	enter();
}

void zero_division() {
	drawWord("ERROR: division by zero");
	commandEnter();
	flag_screenShot = 1;
    
}

void invalid_op() {
	drawWord("ERROR: invalid operation");
	commandEnter();
	flag_screenShot = 1;
}
