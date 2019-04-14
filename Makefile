CC = gcc
ASM = nasm
LD = gcc
CFLAGS =-c -g -O3 -Wall
AFLAGS = -f elf64
LDFLAGS = -g -no-pie
SFML = -lsfml-graphics -lsfml-window -lsfml-system
VPATH = ./src
OBJPATH = ./compile
SRCC = main.c hash.c textproc.c
HEAD = hash.h textproc.h
SRCSASM = 
OBJC = $(SRCC:%.c=$(OBJPATH)/%.o)
OBJASM = $(SRCASM:.S=.o)
EXECUTABLE = run.out

all: $(OBJPATH) $(SRCC) $(SRCASM) $(EXECUTABLE)

$(OBJPATH):
	@mkdir $@

$(EXECUTABLE): $(OBJC)
	@echo "LINKING:"
	$(LD) $(LDFLAGS) $(OBJC) -o  $@ $(SFML)

$(OBJPATH)/%.o: %.c $(VPATH/$(HEAD))
	@echo "COMPILING:"
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(ASM) $(AFLAGS) $< -o $@