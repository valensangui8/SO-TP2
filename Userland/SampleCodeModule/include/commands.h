#ifndef COMMANDS_H
#define COMMANDS_H


#include <syscalls.h>
#include <myStdio.h>
#include <eliminator.h>
#include <itba.h>
#include <invOpcode.h>
#include <test.h>

#define SECONDS 0x0
#define MINUTES 0x2
#define HOURS   0x4
#define DAY     0x7
#define MONTH   0x8
#define YEAR    0x9

void date();

void zoomIn();

void zoomOut();

void clear();

void div0();

void help();

void registers();

void user_test_prio();









#endif