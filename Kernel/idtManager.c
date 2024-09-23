#include <idtManager.h>

static void sys_Read(uint8_t * buf, uint32_t count, uint32_t * size);
static void sys_DrawWord(char * word);
static void sys_DrawChar(char letter);
static void sys_delete();
static void sys_enter();
static void sys_drawError(char * command);
static void sys_commandEnter();
static void sys_zoomIn();
static void sys_zoomOut();
static void sys_clear();
static void sys_getScale(int * scale);
static void sys_drawWithColor(char * word, uint32_t hexColor);
static void sys_drawRegisters();
static void sys_draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color);
static void sys_sleep(unsigned long s);
static void sys_sound(uint32_t nFrequence, uint32_t time);
static void sys_checkHeight(char * HeightPassed, int indexCommand);

void idtManager(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax){
    switch (rax)
    {
    case 0: // read
        sys_Read((uint8_t *) rdi, (uint32_t) rsi, (uint32_t *) rdx); // rdi = buffer ; rsi = size , rdx = count 
        break;
    case 1: // write
        sys_DrawWord((char *) rdi); // rdi = palabra 
        break;
    case 2:
        sys_DrawChar((char) rdi); // rdi = letra 
        break;
    case 3:
        sys_enter(); 
        break;
    case 4:
        sys_delete(); 
        break;
    case 5:
        sys_drawError((char *) rdi);
        break;
    case 6:
        sys_commandEnter();
        break;
    case 7:
        sys_zoomIn();
        break;
    case 8:
        sys_zoomOut();
        break;
    case 9:
        sys_clear();
        break;
    case 10:
        sys_getScale((int *) rdi);
        break;
    case 11:
        sys_drawWithColor((char *) rdi, (uint32_t) rsi);
        break;
    case 12:
        sys_drawRegisters();
        break;
    case 13:
        sys_draw((uint32_t) rdi, (uint32_t) rsi, (uint32_t) rdx, (uint32_t) rcx);
        break;
    case 14:
        sys_sleep((unsigned long) rdi);
        break;
    case 15:
        sys_sound((uint32_t) rdi, (uint32_t) rsi);
        break;
    case 16:
        sys_checkHeight((char *) rdi, (uint32_t) rsi);
        break;
    }

}


void sys_Read(uint8_t * buf, uint32_t count, uint32_t * size){
    readChar(buf, count, size);
}

void sys_DrawWord(char * word){
    drawWord(word);
}

void sys_DrawChar(char letter){
    drawLine(letter);
}

void sys_enter(){
    enter();
}

void sys_delete(){
    delete();
}

void sys_drawError(char * command){
    drawError(command);
}

void sys_commandEnter(){
    commandEnter();
}

void sys_zoomIn(){
    incScale();
}

void sys_zoomOut(){
    decScale();
}

void sys_clear(){
    clear();
}

void sys_getScale(int * scale){
    *scale = getScale();
}

void sys_drawWithColor(char * word, uint32_t hexColor){
    drawWithColor(word, hexColor);
}

void sys_drawRegisters(){
    printRegAsm();
}

void sys_draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color){
    draw(x,y,size,color);
}

void sys_sleep(unsigned long s){
    sleep(s);
}

void sys_sound(uint32_t nFrequence, uint32_t time){
    start_sound(nFrequence, time);
}

void sys_checkHeight(char * HeightPassed, int command) {
    checkHeight(HeightPassed, command);
}