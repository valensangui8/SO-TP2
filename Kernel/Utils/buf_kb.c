#include "buf_kb.h"
static struct buff_kb buffer = {0,0,{0}};
static buff pointer = &buffer;

int getPos(){
    return pointer->write_pos;
}
int newPos(int newPos){
    pointer->write_pos= newPos;
    return newPos;
}
char * getAddress(){
    return pointer->buffer;
}
char getCharAt(int pos){
    return pointer->buffer[pos];
}
void setReadPos(int read_pos){
    pointer->read_pos = read_pos;
}
int getReadPos(){
    return pointer->read_pos;
}
