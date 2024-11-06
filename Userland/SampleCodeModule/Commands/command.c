#include <commands.h>

void zoomIn(){
    int scale;
    call_sys_getScale(&scale);
    if(scale == 4){
        printf("Max zoom in reached");
    }else{
        call_sys_zoomIn();
    }
}

void zoomOut(){
    int scale;
    call_sys_getScale(&scale);
    if(scale == 1){
        printf("Max zoom out reached");
    }else{
        call_sys_zoomOut();
    }
}

void clear(){
    call_sys_clear();
}

void help(){
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

    call_sys_drawWithColor(" testmm <MAX_MERMORY>", 0x32a852);
    printf(" - Executes the memory manager test. Pass the max memory number as an argument.");

    call_sys_drawWithColor(" testprocess <MAX_PROCESSES>", 0x32a852);
    printf(" - Executes the memory manager test. Pass the max number of processes as an argument.");

    call_sys_drawWithColor(" testsync <ITERATIONS> <SEM_VALUE>", 0x32a852);
    printf(" - Executes the synchronization test. Pass the max number of iterations as the first argument and set the use of semaphores as the second.");


    call_sys_drawWithColor(" ps", 0x32a852);
    printf(" - Lists all active processes and their statuses.");

    call_sys_drawWithColor(" kill <PID>", 0x32a852);
    printf(" - Kill process. Pass the PID as an argument.");
}

void div0() {
    int a, b, c;
    a = 0;
    b = 1;
    c = b / a;
    a = c;
}

void registers(){
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

void date() {
    call_sys_drawWord("Current day: ");
    call_sys_drawWord(getDay());

    call_sys_commandEnter();

    call_sys_drawWord("Current time: ");
    call_sys_drawWord(getTime());
}

int test_mm_user(int16_t fds[], int argc, char **argv){
    return call_sys_create_process("testmm", 1, argv, argc, &test_mm, fds);
} 

int test_process_user(int16_t fds[], int argc, char **argv){
    return call_sys_create_process("testprocess", 1, argv, argc, &test_processes, fds);
}


int test_prio_user(int16_t fds[]){
    return call_sys_create_process("testprio", 1, NULL, 0, &test_prio, fds); 
}

void ps(){
    call_sys_list_processes_state();
}

void kill_process(int argc, char **argv) {
    if(call_sys_kill_process(satoi(argv[1])) == 1){
        call_sys_drawWord("You killed the process: ");
        call_sys_draw_int(satoi(argv[1]));
    }
    else{
        call_sys_drawWord("Error killing the process");
    }
}

int test_sync_user(int16_t fds[], int argc, char **argv){
    return call_sys_create_process("testsyncro", 1, argv, argc, &test_sync, fds);
}

static void cat_process(int argc, char **argv) {
    char c;
    while ((c = getChar()) != EOF) { 
        putchar(c);
    }
}

int cat(int16_t fds[], int argc, char **argv){
    return call_sys_create_process("cat", 1, argv, argc, &cat_process, fds); 
}

static void wc_process() {
    char c;
    int line_count = 0;

    while((c = getChar()) != EOF){
        call_sys_drawChar(c);
        if(c == '\n'){
            line_count++;
        }
    }

    printf("Lines: %d", line_count);
}

int wc(int16_t fds[]){
    return call_sys_create_process("wc", 4, NULL, 0, &wc_process, fds);
}

static void filter_process() {
    char c;
    while ((c = getChar()) != EOF) {
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            putchar(c);
        }
    }
    call_sys_enter();
}

int filter(int16_t fds[]){
    return call_sys_create_process("filter", 4, NULL, 0, &filter_process, fds);
}

static void loop_process(int argc, char **argv) {
    int pid = call_sys_get_pid();
    while(1){
        call_sys_sleep(my_atoi(argv[1]) * 1000);
        call_sys_draw_int(pid);
    }
}

int loop(int16_t fds[], int argc, char **argv){
    return call_sys_create_process("loop", 1, argv, argc, &loop_process, fds);
}

void nice(int argc, char **argv){
    call_sys_update_priority(my_atoi(argv[1]), my_atoi(argv[2]));
}

void block_process(int argc, char **argv) {
    call_sys_block_process(my_atoi(argv[1]));
}

void get_memory_info(){
    char type[30];
    uint64_t free, allocated,total;
    call_sys_get_memory_info(type, &free, &allocated, &total);
    call_sys_drawWithColor(type, 0x32a852);
    call_sys_drawWord(" - Free: ");
    free = free / 1024;
    call_sys_draw_int(free);
    call_sys_drawWord("KB -");
    call_sys_drawWord(" Allocated: ");
    allocated = allocated / 1024;
    // allocated = allocated / 2048;
    call_sys_draw_int(allocated);
    call_sys_drawWord("KB");
    call_sys_drawWord("- Total: ");
    total = total / 1024;
    call_sys_draw_int(total);
}

static void text_process() {
    call_sys_drawWord("Hello, I'm a text process\n");
    call_sys_drawWord("I'm going to sleep for 5 seconds\n");
    call_sys_drawWord("I woke up\n");
    call_sys_drawChar(EOF);
    // call_sys_drawWord(EOF);
}

int text(int16_t fds[]){
    return call_sys_create_process("text", 1, NULL, 0, &text_process, fds);
}