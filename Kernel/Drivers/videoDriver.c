#include <videoDriver.h>
#include <font.h>

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
static void uint64HexaToString(uint64_t valorHexa, char *hexaString);
static uint64_t binaryToHex(uint64_t binaryNum);
static void drawLine2(char letter);

static uint32_t characterColor = 0xFFFFFF; // default color white
//static uint32_t colorVariable = 0;
static uint32_t backgroundColor = 0x000000; // default color black

static uint16_t x = 0;
static uint16_t y = 0;
static int scale;
static int flag_enter = 1;
//static int flag_bottom_enter = 0;
static int commands[10] = {0, 0, 0, 21, 21, 31, 23, 4, 0, 0};

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

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
	uint8_t *framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
	uint64_t offset = (x * ((VBE_mode_info->bpp) / 8)) + (y * VBE_mode_info->pitch);
	framebuffer[offset] = (hexColor) & 0xFF;
	framebuffer[offset + 1] = (hexColor >> 8) & 0xFF;
	framebuffer[offset + 2] = (hexColor >> 16) & 0xFF;
}

/////////////////DRAW////////////////////

// draws a square with the given color, width, height and position
void drawSquare(uint32_t hexColor, uint64_t width, uint64_t height, int x, int y) {
	for (uint64_t i = x; i - x < width; i++) {
		for (uint64_t j = y; j - y < height; j++) {
			putPixel(hexColor, i, j);
		}
	}
}

// draws a square with the given color, size and position
void draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color) {
	drawSquare(color, size, size, x, y);
}

// draw registers in the screen
void drawRegisters(uint64_t value) {
	char buffer[256] = {0};
	value = binaryToHex(value);
	uint64HexaToString(value, buffer);
	drawWord(buffer);
	commandEnter();
}

// transform a binary number to a hexadecimal number
static uint64_t binaryToHex(uint64_t binaryNum) {
	return binaryNum;
}

// transform a hexadecimal number to a string
static void uint64HexaToString(uint64_t valorHexa, char *hexaString) {
	int i;
	for (i = 15; i >= 0; i--) {
		uint64_t nibble = (valorHexa >> (i * 4)) & 0xF;
		hexaString[15 - i] = (nibble < 10) ? (char) ('0' + nibble) : (char) ('A' + (nibble - 10));
	}
	hexaString[16] = 'h';
	hexaString[17] = '\0';
}

// draw a character
void drawChar(char character) {
	char *bitMapChar = font[character];
	drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y);
	if(character == '\t'){
		x += TAB_SIZE * 8 * scale;
		return;
	}
	else if(character == '\n'){
		commandEnter();
		return;
	}else if(character == '\b'){
		delete();
		return;
	} else if(character == EOF){
		return;
	}
	for (int i = 0; i < HEIGHT_FONT * scale; i++) {
		for (int j = 0; j < WIDTH_FONT * scale; j++) {
			int bit = (bitMapChar[i / scale] >> (j / scale)) & 1;
			if (bit) {
				putPixel(characterColor, x + j, y + i);
			}
		}
	}
	x += WIDTH_FONT * scale;
}

// draw error message after an unavailable command
void drawError(char *word) {
	drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y);
	commandEnter();
	char *toDraw = "ERROR - command not found: ";
	drawWord(toDraw);
	drawWord(word);
	enter();
}

// function to draw a word
void drawWord(char *word) {
	int i = 0;
	while (word[i] != 0) {
		drawLine2(word[i]);
		i++;
	}
}

// function to draw a line modified without updating cursor
static void drawLine2(char letter) {
	if (x + 8 * scale >= VBE_mode_info->width) {
		drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y);
		x = 0;
		y += 16 * scale;
		flag_enter = 0;
	}
	if (y + 16 * scale >= VBE_mode_info->height) {
		y = 0;
		clear();
		flag_enter = 1;
		return;
	}
	drawChar(letter);
}

// function to draw a line when a character reaches the end of the X coordinates
void drawLine(char letter) {
	if (x + 8 * scale >= VBE_mode_info->width) {
		drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y);
		x = 0;
		y += 16 * scale;
		flag_enter = 0;
	}
	if (y + 16 * scale >= VBE_mode_info->height) {
		y = 0;
		clearScreen();
		flag_enter = 1;
		return;
	}
	drawChar(letter);
	updateCursor();
}

void drawInt(int num) {
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
	drawWord(buffer);
}

void drawHex(uint64_t value) {
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
	drawWord(buffer);
}

// function to update the cursor position
void updateCursor() {
	drawSquare(characterColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y);
}

// function to draw a word with a given color
void drawWithColor(char *word, uint32_t hexColor) {
	characterColor = hexColor;
	drawWord(word);
	characterColor = WHITE;
}

/////////////////CLEAR////////////////////

// function to clear the screen, printing the initial message
void clearScreen() {
	drawSquare(backgroundColor, VBE_mode_info->width, VBE_mode_info->height, 0, 0);
	x = 0;
	y = 0;
	drawWord("TP-SO-Grupo-10$");
}

// function to clear the screen entirely
void clear() {
	drawSquare(backgroundColor, VBE_mode_info->width, VBE_mode_info->height, 0, 0);
	x = 0;
	y = 0;
	updateAfterCommand();
}

/////////////////INITIALIZE////////////////////

// function to start the screen
void start() {
	scale = 3;
	x = 435;
	y = 350;
	drawWord("voidOS");
	scale = 1;
	x -= 100;
	y += 50;
	drawWord("Loading...");
}

// function to initialize the screen
void initialize() {
	scale = 1;
	x = 0;
	y = 0;
	drawWord("TP-SO-Grupo-10$ ");
	updateCursor();
}

/////////////////ENTER////////////////////

// function to do an enter
void enter() {
	drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y);
	y += 16 * scale;
	x = 0;
	drawWord("TP-SO-Grupo-10$");
	flag_enter = 1;
	x += 8 * scale;
	updateCursor();

	return;
}

// function to do an enter after a command
void commandEnter() {
	drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y);
	x = 0;
	y += HEIGHT_FONT * scale;
}

/////////////////SCALE////////////////////

// function to get the scale of the font
int getScale() {
	return scale;
}

// function to increase the scale of the font
void incScale() {
	scale++;
	clear();
}

// function to decrease the scale of the font
void decScale() {
	if (scale > 1) {
		scale--;
		clear();
	}
}

// function to update the position of the coordinates after a command
void updateAfterCommand() {
	x -= WIDTH_FONT * scale;
	y -= HEIGHT_FONT * scale;
}

/////////////////DELETE////////////////////

// function to delete a character
void delete() {
	if (x <= 16 * 8 * scale && flag_enter == 1) {
		return;
	}
	if (x < WIDTH_FONT * scale) {
		drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y); // borro puntero linea de abajo
		x = VBE_mode_info->width - 2 * (WIDTH_FONT * scale);						// vuelvo a último lugar de la línea en X
		y -= HEIGHT_FONT * scale;													// vuelvo un renglón para arriba
		drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y); // borro letra de linea arriba der
		updateCursor();
		return;
	}
	drawSquare(backgroundColor, WIDTH_FONT * scale, HEIGHT_FONT * scale, x, y);
	x -= WIDTH_FONT * scale;
	updateCursor();
	return;
}

/////////////////CHECK////////////////////

// function to check if the command is too long
void checkHeight(char *HeightPassed, int command) {
	if (y + commands[command] * 16 * scale >= VBE_mode_info->height) {
		*HeightPassed = 1;
	}
	else {
		*HeightPassed = 0;
	}
}
