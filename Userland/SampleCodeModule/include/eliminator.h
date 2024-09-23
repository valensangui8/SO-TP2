#ifndef ELIMINATOR_H
#define ELIMINATOR_H

#include <myStdio.h>
#include <stdint.h>
#include <syscalls.h>


#define MAX_Y 768
#define MAX_X 1024

#define RED 0x9C1F14
#define GREEN 0x4CA730

#define SQUARE_SIZE 16
#define X_SQUARES MAX_X / SQUARE_SIZE // 32
#define Y_SQUARES MAX_Y / SQUARE_SIZE


#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


typedef struct player{
    int x;
    int y;
    int dir;
    int score;
    uint32_t color;
} player;




void eliminator();

void printMessage();

void drawBorders();

void startGame();

void startPlayer();

void movePlayer(player * p, player * otherPlayer, char * flag);

void displayScore(player * p1, player * p2);

void clearMatrix();

void chatGPT(player * p);

#endif