#include <commands.h>

void zoomIn(int background){
    int scale;
    call_sys_getScale(&scale);
    if(scale == 4){
        printf("Max zoom in reached");
    }else{
        call_sys_zoomIn();
    }
}
void zoomOut(int background){
    int scale;
    call_sys_getScale(&scale);
    if(scale == 1){
        printf("Max zoom out reached");
    }else{
        call_sys_zoomOut();
    }
}

void clear(int background){
    call_sys_clear();
}

void help(int background){
    call_sys_commandEnter();
    
    printf("PROGRAMS AVAILABLE FOR USER: ");
    
    
    call_sys_drawWithColor(" help", 0x32a852);
    printf(" - shows the different programs available.");
    

    call_sys_drawWithColor(" date", 0x32a852);
    printf(" - Retrieve the current day and time.");
   
    
    call_sys_drawWithColor(" eliminator", 0x32a852);
    printf(" - Play the Eliminator game, available for both single-player and two-player modes.");
  

    call_sys_drawWithColor(" clear", 0x32a852);
    printf(" - Clear the screen.");
   

    call_sys_drawWithColor(" zoomIn", 0x32a852);
    printf(" - Increase text size. Clears screen after change.");


    call_sys_drawWithColor(" zoomOut", 0x32a852);
    printf(" - Decrease text size. Clears screen after change.");
  

    call_sys_drawWithColor(" registers", 0x32a852);
    printf(" - View the registers in use after pressing ',' .");

    call_sys_drawWithColor(" itba", 0x32a852);
    printf(" - We'll show you ITBA's logo after execution.");
 

    call_sys_drawWithColor(" EXCEPTIONS", 0xc70e24);
    call_sys_commandEnter();

    call_sys_drawWithColor(" invalidOpcode", 0x32a852);
    printf(" - Indicates an invalid operation.");
   
    call_sys_drawWithColor(" div0", 0x32a852);
    printf(" - Signals division by zero.");

    printf(" TEST:");

    call_sys_drawWithColor(" testprio", 0x32a852);
    printf(" - Executes the memory manager test.");

    call_sys_drawWithColor(" testmm", 0x32a852);
    printf(" - Executes the memory manager test. Pass the max memory number as an argument.");

    call_sys_drawWithColor(" testprocess", 0x32a852);
    printf(" - Executes the memory manager test. Pass the max number of processes as an argument.");

    call_sys_drawWithColor(" testsync", 0x32a852);
    printf(" - Executes the synchronization test. Pass the max number of iterations as the first argument and set the use of semaphores as the second.");


    call_sys_drawWithColor(" ps", 0x32a852);
    printf(" - Lists all active processes and their statuses.");

    call_sys_drawWithColor(" kill", 0x32a852);
    printf(" - Kill process. Pass the PID as an argument.");

    
}

void div0(int background) {
    int a, b, c;
    a = 0;
    b = 1;
    c = b / a;
    a = c;
}

void registers(int background){
    call_sys_drawRegisters();
}




/*DATE*/

char timeBuffer[5] = {'\0'};
char dayBuffer[8] = {'\0'};

unsigned int decode(unsigned int time){
    return (time >> 4) * 10 + (time & 0x0F);				//Funcion que pasa el tiempo en BCD(binary coded decimal) a un numero en decimal
}
static uint32_t numberToBase(uint64_t value, char * buffer, uint32_t base) {
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    do {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}
void buildTwoDigitsData(char* buffer, int dataTime) {
    if (dataTime < 10) {
        buffer[0] = '0';
        numberToBase(dataTime, buffer + 1, 10);
    } else {
        numberToBase(dataTime, buffer, 10);
    }
}
char* getTime(){
    int hours = decode(call_sys_get_date(HOURS)) - 3;
    int minutes = decode(call_sys_get_date(MINUTES));

    buildTwoDigitsData(timeBuffer, hours);
    timeBuffer[2] = ':';
    buildTwoDigitsData(timeBuffer + 3, minutes);
    
    return timeBuffer;
}

char * getDay() {
    int day = decode(call_sys_get_date(DAY));
    int month = decode(call_sys_get_date(MONTH));
    int year = decode(call_sys_get_date(YEAR));

    buildTwoDigitsData(dayBuffer, day);
    dayBuffer[2] = '/';
    buildTwoDigitsData(dayBuffer + 3, month);
    dayBuffer[5] = '/';
    buildTwoDigitsData(dayBuffer + 6, year);

    return dayBuffer;
}

void date(int background) {
    call_sys_drawWord("Current day: ");
    call_sys_drawWord(getDay());

    call_sys_commandEnter();

    call_sys_drawWord("Current time: ");
    call_sys_drawWord(getTime());
}

void test_mm_user(int background, int argc, char **argv){
    call_sys_create_process("testmm", 1, 0, argv, argc, &test_mm);
    
} 

void test_process_user(int background, int argc, char **argv){
    call_sys_create_process("testprocess", 1, background, argv, argc, &test_processes);
}


void test_prio_user(int background){
    call_sys_create_process("testprio", 1, background, NULL, 0, &test_prio);
    
}

void ps(int background){
    call_sys_list_processes_state();
}

void kill_process(int background, int argc, char **argv) {
    if(call_sys_kill_process(satoi(argv[1])) == 1){
        call_sys_drawWord("You killed the process: ");
        call_sys_draw_int(satoi(argv[1]));
    }
    else{
        call_sys_drawWord("Error killing the process");
    }
}

void test_sync_user(int background, int argc, char **argv){
    call_sys_create_process("testsyncro", 1, background, argv, argc, &test_sync);
}