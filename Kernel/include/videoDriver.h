#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <naiveConsole.h>
#include <stdint.h>
#include <lib.h>

/////////////////DRAW////////////////////

void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y);						   // draws a pixel with the given color and position
void draw_square(uint32_t hexColor, uint64_t width, uint64_t height, int x, int y); // draws a square with the given color, width, height and position
void draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color);				   // draws a square with the given color, size and position
void draw_registers(uint64_t value);												   // draw registers in the screen
void draw_char(char character);												   // draw a character
void draw_error(char *word);														   // draw an error message after an unavailable command
void draw_word(char *word);														   // draw a word in the screen
void draw_line(char letter);														   // draw a line in the screen
void update_cursor();															   // update the cursor position
void draw_with_color(char *word, uint32_t hexColor);								   // draw a word with a given color
void draw_int(int num);															   // draw an integer
void draw_hex(uint64_t value);													   // print a hexadecimal number

/////////////////CLEAR////////////////////

void clear_screen(); // function to clear the screen, printing the initial message
void clear();		// function to clear the screen entirely

/////////////////INITIALIZE////////////////////

void start();	   // function to start the screen
void initialize(); // function to initialize the screen

/////////////////ENTER////////////////////

void enter();		 // function to do an enter
void command_enter(); // function to do an enter after a command

/////////////////SCALE////////////////////

int get_scale();			   // function to get the scale of the font
void inc_scale();		   // function to increase the scale of the font
void dec_scale();		   // function to decrease the scale of the font
void update_after_command(); // function to update the position of the coordinates after a command

/////////////////DELETE///////////////////

void delete(); // function to delete a character

/////////////////CHECK///////////////////

void check_height(char *heightPassed, int indexCommand); // function to check if the command is too long

#endif