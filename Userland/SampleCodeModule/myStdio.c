#include <syscalls.h>
#include <stdint.h>
#include <stdarg.h>
#include <myStdio.h>


char getChar(){
    uint8_t c;
    uint32_t size = 0;
    while(size == 0){
        call_sys_read(&c,1,&size);
        if(size == 0){
            call_sys_yield();  
        }
    }
    return c;
}

int strcmp(const char * s1, const char * s2) {
    int i = 0;
    while(s1[i] != 0  && s2[i] != 0){
        if(s1[i] != s2[i]){
            return s1[i] - s2[i];
        }
        i++;
    }
    return s1[i]-s2[i];
}


int strcpy(char * dest, char * src){
    int i = 0;
    while(src[i]!=0){
        dest[i] = src[i];
        i++;
    }
    dest[i]=0;
    return i;

}

void printf(char * fmt, ...){
    va_list variables;

    va_start(variables, fmt);
    char toPrint[512];
    unsigned int index=0;
    while(*fmt != 0){
        if(*fmt == '%'){
            fmt++;
            switch (*fmt){
            case 'd':
                index += intToString(va_arg(variables,int),toPrint+index);
                break;
            case 's':
                index += strcpy(toPrint+index,va_arg(variables,char *));
            
            default:
                break;
            }
            fmt++;
        } else toPrint[index++]=*fmt++; 
    }
    toPrint[index]=0;
    puts(toPrint);
    va_end(variables);
}

void puts(char * buffer){
    call_sys_drawWord(buffer);
    call_sys_commandEnter();
    call_sys_commandEnter();
}

void putchar(char c){
    call_sys_drawChar(c);
}

int intToString(int num, char *toPrint){
    int i=0, j=0;
    int negativo = 0;
    char aux[10]; 

    if (num < 0) {
        negativo = 1;
        num = -num;
    }

    if (num == 0) {
        toPrint[i] = '0';
        i++;
    } 

    while (num != 0) {
        aux[j] = (num % 10) + '0';
        num = num / 10;
        j++;
    }

    if (negativo) {
        toPrint[i] = '-';
        i++;
    }

    for (j = j - 1; j >= 0; j--) {
        toPrint[i] = aux[j];
        i++;
    }
    toPrint[i] = '\0';

    return i;
}