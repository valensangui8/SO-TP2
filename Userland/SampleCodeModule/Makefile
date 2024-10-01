include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c)
MODULEELF=0000-sampleCodeModule.elf
SOURCES_ASM=$(wildcard */*.asm)
SOURCES_COMMANDS=$(wildcard */*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_COMMANDS=$(SOURCES_COMMANDS:.c=.o)


all: $(MODULE)

$(MODULE):$(OBJECTS) $(OBJECTS_ASM)  $(OBJECTS_COMMANDS)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_COMMANDS) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 -no-pie $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_COMMANDS) -o ../$(MODULEELF)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@




clean:
	rm -rf */*.o *.o *.bin

.PHONY: all clean print
