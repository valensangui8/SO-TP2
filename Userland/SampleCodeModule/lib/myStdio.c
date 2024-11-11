#include <myStdio.h>

char get_char(){
    char c;
    uint32_t size = 0;
    while(size == 0){
        call_sys_read(&c, 1, &size);
    }
    return c;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
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
                index += int_to_string(va_arg(variables,int),toPrint+index);
                break;
            case 's':
                index += strcpy(toPrint+index,va_arg(variables,char *));
                break;
            case 'p':
                index += hex_to_string(va_arg(variables,uint64_t),toPrint+index);
                break;
            case 'c':
                toPrint[index++]=va_arg(variables,int);
                break;
            default:
                break;
            }
            fmt++;
        } else toPrint[index++]=*fmt++; 
    }
    toPrint[index]=0;
    call_sys_draw_word(toPrint);
    // puts(toPrint);
    va_end(variables);
}

void puts(char * buffer){
    call_sys_draw_word(buffer);
    call_sys_command_enter();
}

void put_char(char c) {
    call_sys_draw_char(c);
}

int int_to_string(int num, char *toPrint){
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

int hex_to_string(uint64_t num, char *toPrint) {
    char hexa[16] = "0123456789ABCDEF";
    char aux[16];
    int i = 0, j = 0;

    if (num == 0) {
        toPrint[i] = '0';
        i++;
    }

    while (num != 0) {
        aux[j] = hexa[num % 16];
        num = num / 16;
        j++;
    }

    toPrint[i] = '0';
    i++;
    toPrint[i] = 'x';
    i++;

    for (j = j - 1; j >= 0; j--) {
        toPrint[i] = aux[j];
        i++;
    }
    toPrint[i] = '\0';

    return i;
}

char * strtok(char * str, const char * delim){
    static char * last = NULL;
    if(str != NULL){
        last = str;
    }
    if(last == NULL){
        return NULL;
    }
    char * ret = last;
    while(*last != 0){
        if(*last == *delim){
            *last = 0;
            last++;
            return ret;
        }
        last++;
    }
    last = NULL;
    return ret;
}


char *my_itoa(int num) {
	static char str[12];
	int i = 0;
	int isNegative = 0;
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	if (num < 0) {
		isNegative = 1;
		num = -num;
	}
	while (num != 0) {
		int rem = num % 10;
		str[i++] = rem + '0';
		num = num/10;
	}
	if (isNegative) {
		str[i++] = '-';
	}
	str[i] = '\0';
	int start = 0;
	int end = i - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
	return str;
}

int my_atoi(char *str) {
    int res = 0;
    int i = 0;
    int sign = 1;
    if (str[0] == '-') {
        sign = -1;
        i++;
    }
    for (; str[i] != '\0'; ++i) {
        res = res * 10 + str[i] - '0';
    }
    return sign * res;
}