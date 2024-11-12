// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <font.h>
#include <videoDriver.h>

// font
#define WIDTH_FONT 8
#define HEIGHT_FONT 16
#define TAB_SIZE 4
#define MAX_X 1008
#define MAX_Y 736
#define MIN_X 8
#define MIN_Y 8

// screen
#define LAST_X 1024
#define LAST_Y 768
#define SCALE 1

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define YELLOW 0xFFFF00
#define ORANGE 0xFFA500
static void uint64_hexa_to_string(uint64_t valorHexa, char *hexaString);
static uint64_t binary_to_hex(uint64_t binaryNum);
static void draw_line2(char letter);

static uint32_t characterColor = 0xFFFFFF;	// default color white
static uint32_t backgroundColor = 0x000000; // default color black

static uint16_t x_index = 0;
static uint16_t y_index = 0;
static int scale;
static int flag_enter = 1;
static int commands[10] = {0, 0, 0, 21, 21, 68, 23, 4, 0, 0};

struct vbe_mode_info_structure {
	uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;	  // deprecated
	uint8_t window_b;	  // deprecated
	uint16_t granularity; // deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;		   // number of bytes per horizontal line
	uint16_t width;		   // width in pixels
	uint16_t height;	   // height in pixels
	uint8_t w_char;		   // unused...
	uint8_t y_char;		   // ...
	uint8_t planes;
	uint8_t bpp;   // bits per pixel in this mode
	uint8_t banks; // deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__((packed));

typedef struct vbe_mode_info_structure *VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y) {
	uint8_t *framebuffer = (uint8_t *) (uintptr_t) VBE_mode_info->framebuffer;
	uint64_t offset = (x * ((VBE_mode_info->bpp) / 8)) + (y * VBE_mode_info->pitch);
	framebuffer[offset] = (hexColor) & 0xFF;
	framebuffer[offset + 1] = (hexColor >> 8) & 0xFF;
	framebuffer[offset + 2] = (hexColor >> 16) & 0xFF;
}

/////////////////DRAW////////////////////

// draws a square with the given color, width, height and position
void draw_square(uint32_t hexColor, uint64_t width, uint64_t height, int x, int y) {
	for (uint64_t i = x; i - x < width; i++) {
		for (uint64_t j = y; j - y < height; j++) {
			put_pixel(hexColor, i, j);
		}
	}
}

// draws a square with the given color, size and position
void draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color) {
	draw_square(color, size, size, x, y);
}

// draw registers in the screen
void draw_registers(uint64_t value) {
	char buffer[256] = {0};
	value = binary_to_hex(value);
	uint64_hexa_to_string(value, buffer);
	draw_word(buffer);
	command_enter();
}

// transform a binary number to a hexadecimal number
static uint64_t binary_to_hex(uint64_t binaryNum) {
	return binaryNum;
}

// transform a hexadecimal number to a string
static void uint64_hexa_to_string(uint64_t valorHexa, char *hexaString) {
	int i;
	for (i = 15; i >= 0; i--) {
		uint64_t nibble = (valorHexa >> (i * 4)) & 0xF;
		hexaString[15 - i] = (nibble < 10) ? (char) ('0' + nibble) : (char) ('A' + (nibble - 10));
	}
	hexaString[16] = 'h';
	hexaString[17] = '\0';
}

// draw a character
void draw_char(char character) {
	unsigned char *bitMapChar = font[(unsigned char) character];
	draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index);
	if (character == '\t') {
		x_index += TAB_SIZE * 8 * scale;
		return;
	}
	else if (character == '\n') {
		command_enter();
		return;
	}
	else if (character == '\b') {
		delete ();
		return;
	}
	else if ((int) character == EOF) {
		return;
	}
	for (int i = 0; i < HEIGHT_FONT * scale; i++) {
		for (int j = 0; j < WIDTH_FONT * scale; j++) {
			int bit = (bitMapChar[i / scale] >> (j / scale)) & 1;
			if (bit) {
				put_pixel(characterColor, x_index + j, y_index + i);
			}
		}
	}
	x_index += WIDTH_FONT * scale;
}

// draw error message after an unavailable command
void draw_error(char *word) {
	draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index);
	command_enter();
	char *toDraw = "ERROR - command not found: ";
	draw_word(toDraw);
	draw_word(word);
	enter();
}

// function to draw a word
void draw_word(char *word) {
	int i = 0;
	while (word[i] != 0) {
		draw_line2(word[i]);
		i++;
	}
}

// function to draw a line modified without updating cursor
static void draw_line2(char letter) {
	if (x_index + 8 * scale >= VBE_mode_info->width) {
		draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index);
		x_index = 0;
		y_index += 16 * scale;
		flag_enter = 0;
	}
	if (y_index + 16 * scale >= VBE_mode_info->height) {
		y_index = 0;
		clear();
		flag_enter = 1;
		return;
	}
	draw_char(letter);
}

// function to draw a line when a character reaches the end of the X coordinates
void draw_line(char letter) {
	if (x_index + 8 * scale >= VBE_mode_info->width) {
		draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index);
		x_index = 0;
		y_index += 16 * scale;
		flag_enter = 0;
	}
	if (y_index + 16 * scale >= VBE_mode_info->height) {
		y_index = 0;
		clear_screen();
		flag_enter = 1;
		return;
	}
	draw_char(letter);
	update_cursor();
}

void draw_int(int num) {
	char buffer[256] = {0};
	int i = 0;
	if (num == 0) {
		buffer[i++] = '0';
	}
	while (num > 0) {
		buffer[i++] = num % 10 + '0';
		num /= 10;
	}
	buffer[i] = 0;
	i--;
	int j = 0;
	while (i > j) {
		char aux = buffer[i];
		buffer[i] = buffer[j];
		buffer[j] = aux;
		i--;
		j++;
	}
	draw_word(buffer);
}

void draw_hex(uint64_t value) {
	char buffer[256] = {0};
	int i = 0;
	// Agregar el prefijo "0x"
	buffer[i++] = '0';
	buffer[i++] = 'x';
	if (value == 0) {
		buffer[i++] = '0';
	}
	else {
		// Convertir el valor a hexadecimal
		while (value > 0) {
			uint64_t nibble = value & 0xF;
			buffer[i++] = (nibble < 10) ? (char) ('0' + nibble) : (char) ('A' + (nibble - 10));
			value >>= 4;
		}
	}
	buffer[i] = 0; // Terminar la cadena
	// Invertir el contenido del buffer (sin contar "0x")
	int start = 2; // Comenzar después de "0x"
	int end = i - 1;
	while (end > start) {
		char aux = buffer[end];
		buffer[end] = buffer[start];
		buffer[start] = aux;
		end--;
		start++;
	}
	draw_word(buffer);
}

// function to update the cursor position
void update_cursor() {
	draw_square(characterColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index);
}

// function to draw a word with a given color
void draw_with_color(char *word, uint32_t hexColor) {
	characterColor = hexColor;
	draw_word(word);
	characterColor = WHITE;
}

/////////////////CLEAR////////////////////

// function to clear the screen, printing the initial message
void clear_screen() {
	draw_square(backgroundColor, VBE_mode_info->width, VBE_mode_info->height, 0, 0);
	x_index = 0;
	y_index = 0;
	draw_word("TP-SO-Grupo-10$");
}

// function to clear the screen entirely
void clear() {
	draw_square(backgroundColor, VBE_mode_info->width, VBE_mode_info->height, 0, 0);
	x_index = 0;
	y_index = 0;
	update_after_command();
}

/////////////////INITIALIZE////////////////////

// function to start the screen
void start() {
	scale = 3;
	x_index = 435;
	y_index = 350;
	draw_word("voidOS");
	scale = 1;
	x_index -= 100;
	y_index += 50;
	draw_word("Loading...");
}

// function to initialize the screen
void initialize() {
	scale = 1;
	x_index = 0;
	y_index = 0;
	draw_word("TP-SO-Grupo-10$ ");
	update_cursor();
}

/////////////////ENTER////////////////////

// function to do an enter
void enter() {
	draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index);
	y_index += 16 * scale;
	x_index = 0;
	draw_word("TP-SO-Grupo-10$");
	flag_enter = 1;
	x_index += 8 * scale;
	update_cursor();

	return;
}

// function to do an enter after a command
void command_enter() {
	draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index);
	x_index = 0;
	y_index += HEIGHT_FONT * scale;
}

/////////////////SCALE////////////////////

// function to get the scale of the font
int get_scale() {
	return scale;
}

// function to increase the scale of the font
void inc_scale() {
	scale++;
	clear();
}

// function to decrease the scale of the font
void dec_scale() {
	if (scale > 1) {
		scale--;
		clear();
	}
}

// function to update the position of the coordinates after a command
void update_after_command() {
	x_index -= WIDTH_FONT * scale;
	y_index -= HEIGHT_FONT * scale;
}

/////////////////DELETE////////////////////

// function to delete a character
void delete() {
	if (x_index <= 16 * 8 * scale && flag_enter == 1) {
		return;
	}
	if (x_index < WIDTH_FONT * scale) {
		draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index); // borro puntero linea de abajo
		x_index = VBE_mode_info->width - 2 * (WIDTH_FONT * scale);								 // vuelvo a último lugar de la línea en X
		y_index -= HEIGHT_FONT * scale;															 // vuelvo un renglón para arriba
		draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index); // borro letra de linea arriba der
		update_cursor();
		return;
	}
	draw_square(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x_index, y_index);
	x_index -= WIDTH_FONT * scale;
	update_cursor();
	return;
}

/////////////////CHECK////////////////////

// function to check if the command is too long
void check_height(char *HeightPassed, int command) {
	if (command < 10 && y_index + commands[command] * 16 * scale >= VBE_mode_info->height) {
		*HeightPassed = 1;
	}
	else {
		*HeightPassed = 0;
	}
}
