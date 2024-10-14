#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>
#include <naiveConsole.h>


/////////////////DRAW////////////////////

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y); // draws a pixel with the given color and position
void drawSquare(uint32_t hexColor, uint64_t width, uint64_t height, int x, int y); // draws a square with the given color, width, height and position
void draw(uint32_t x, uint32_t y, uint32_t size, uint32_t color); // draws a square with the given color, size and position
void drawRegisters(uint64_t value); // draw registers in the screen
void drawChar(uint8_t character); // draw a character
void drawError(char *word); // draw an error message after an unavailable command
void drawWord(char * word); // draw a word in the screen
void drawLine(char letter); // draw a line in the screen
void updateCursor(); // update the cursor position
void drawWithColor(char * word, uint32_t hexColor); // draw a word with a given color
void drawInt(int num); // draw an integer

/////////////////CLEAR////////////////////

void clearScreen(); // function to clear the screen, printing the initial message
void clear(); // function to clear the screen entirely

/////////////////INITIALIZE////////////////////

void start(); // function to start the screen
void initialize(); // function to initialize the screen

/////////////////ENTER////////////////////

void enter(); // function to do an enter
void commandEnter(); // function to do an enter after a command

/////////////////SCALE////////////////////

int getScale(); // function to get the scale of the font
void incScale(); // function to increase the scale of the font
void decScale(); // function to decrease the scale of the font
void updateAfterCommand(); // function to update the position of the coordinates after a command

/////////////////DELETE///////////////////

void delete(); // function to delete a character

/////////////////CHECK///////////////////

void checkHeight(char * heightPassed, int indexCommand); // function to check if the command is too long

#endif