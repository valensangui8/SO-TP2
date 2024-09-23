include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_COMMANDS=$(wildcard commands/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_COMMANDS=$(SOURCES_COMMANDS:.c=.o)


all: $(MODULE)

$(MODULE):$(OBJECTS) $(OBJECTS_ASM)  $(OBJECTS_COMMANDS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_COMMANDS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@




clean:
	rm -rf */*.o *.o *.bin

.PHONY: all clean print
