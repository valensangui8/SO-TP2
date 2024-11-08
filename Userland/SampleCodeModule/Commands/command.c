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
    
    printf("PROGRAMS AVAILABLE FOR USER: \n");
    
    
    call_sys_drawWithColor(" help\n", 0x32a852);
    printf(" - shows the different programs available.\n");
    

    call_sys_drawWithColor(" date\n", 0x32a852);
    printf(" - Retrieve the current day and time.\n");
   
    
    call_sys_drawWithColor(" eliminator\n", 0x32a852);
    printf(" - Play the Eliminator game, available for both single-player and two-player modes.\n");
  

    call_sys_drawWithColor(" clear\n", 0x32a852);
    printf(" - Clear the screen.");
   

    call_sys_drawWithColor(" zoomIn\n", 0x32a852);
    printf(" - Increase text size. Clears screen after change.\n");


    call_sys_drawWithColor(" zoomOut\n", 0x32a852);
    printf(" - Decrease text size. Clears screen after change.\n");
  

    call_sys_drawWithColor(" registers\n", 0x32a852);
    printf(" - View the registers in use after pressing ',' .\n");

    call_sys_drawWithColor(" itba\n", 0x32a852);
    printf(" - We'll show you ITBA's logo after execution.\n");
 

    call_sys_drawWithColor(" EXCEPTIONS\n", 0xc70e24);
    call_sys_commandEnter();

    call_sys_drawWithColor(" invalidOpcode\n", 0x32a852);
    printf(" - Indicates an invalid operation.\n");
   
    call_sys_drawWithColor(" div0\n", 0x32a852);
    printf(" - Signals division by zero.\n");

    printf(" TEST:\n");

    call_sys_drawWithColor(" testprio\n", 0x32a852);
    printf(" - Executes the memory manager test.\n");

    call_sys_drawWithColor(" testmm <MAX_MERMORY>\n", 0x32a852);
    printf(" - Executes the memory manager test. Pass the max memory number as an argument.\n");

    call_sys_drawWithColor(" testprocess <MAX_PROCESSES>\n", 0x32a852);
    printf(" - Executes the memory manager test. Pass the max number of processes as an argument.\n");

    call_sys_drawWithColor(" testsync <ITERATIONS> <SEM_VALUE>\n", 0x32a852);
    printf(" - Executes the synchronization test. Pass the max number of iterations as the first argument and set the use of semaphores as the second.\n");


    call_sys_drawWithColor(" ps\n", 0x32a852);
    printf(" - Lists all active processes and their statuses.\n");

    call_sys_drawWithColor(" kill <PID>\n", 0x32a852);
    printf(" - Kill process. Pass the PID as an argument.\n");
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
    int pids[MAX_PROCESS];
    char states[MAX_PROCESS][10];
    uint64_t rsps[MAX_PROCESS];
    uint64_t rbps[MAX_PROCESS];
    char commands[MAX_PROCESS][30];
    int process_count = 0;

    call_sys_list_processes_state(pids, states, rsps, rbps, commands, &process_count);

    printf("\nSTATE: S = Ready, T = Blocked, R = Running, Z = Zombie \t PRIORITY LEVELS: N = Low, M = Medium, H = High, < = Very High\n");
    printf("BACKGROUND: +   \t  SESSION LEADER: s\n\n");

    printf("PID        STAT          RSP           RBP         COMMAND\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < process_count; i++) {
        printf("%d           %s       %p      %p       %s\n", pids[i], states[i], rsps[i], rbps[i], commands[i]);
    }
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
    int line_count = 0, word_count = 0, char_count = 0, in_word = 0;

    while((c = getChar()) != EOF){
        call_sys_drawChar(c);
        if(c == '\n'){
            line_count++;
        }
        if(!in_word && c != ' ' && c != '\n' && c != '\t'){
            in_word = 1;
            word_count++;
        }else{
            in_word = 0;
        }
        char_count++;
    }

    printf("Lines: %d  Word: %d  Characters: %d", line_count, word_count, char_count);
}

int wc(int16_t fds[]){
    int pid = call_sys_create_process("wc", 4, NULL, 0, &wc_process, fds);
    call_sys_wait_children(pid);
    return pid;
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
    call_sys_draw_int(allocated);
    call_sys_drawWord("KB");
    call_sys_drawWord("- Total: ");
    total = total / 1024;
    call_sys_draw_int(total);
    call_sys_drawWord("KB");
}

static void text_process() {
    call_sys_drawWord("Hello, I'm a text process\n");
    call_sys_drawWord("I'm going to sleep for 5 seconds\n");
    call_sys_drawWord("I woke up\n");
    call_sys_drawChar(EOF);
}

int text(int16_t fds[]){
    return call_sys_create_process("text", 1, NULL, 0, &text_process, fds);
}