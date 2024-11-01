#ifndef COMMANDS_H
#define COMMANDS_H


#include <syscalls.h>
#include <myStdio.h>
#include <eliminator.h>
#include <itba.h>
#include <invOpcode.h>
#include <test.h>
#include <test_util.h>

#define SECONDS 0x0
#define MINUTES 0x2
#define HOURS   0x4
#define DAY     0x7
#define MONTH   0x8
#define YEAR    0x9

void date(int background);

void zoomIn(int background);

void zoomOut(int background);

void clear(int background);

void div0(int background);

void help(int background);

void registers(int background);

void test_mm_user(int background, int argc, char **argv);

void test_process_user(int background,int argc, char **argv);

void test_prio_user(int background);

void test_sync_user(int background,int argc, char **argv);

void ps(int background);

void kill_process(int background,int argc, char **argv);

#endif