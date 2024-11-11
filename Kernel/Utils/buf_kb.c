#include "buf_kb.h"
static struct buff_kb buffer = {0,0,0};
static buff pointer = &buffer;

int get_pos(){
    return pointer->write_pos;
}
int newPos(int newPos){
    pointer->write_pos= newPos;
    return newPos;
}
char * get_address(){
    return pointer->buffer;
}
char get_charAt(int pos){
    return pointer->buffer[pos];
}
void set_read_pos(int read_pos){
    pointer->read_pos = read_pos;
}
int get_read_pos(){
    return pointer->read_pos;
}
