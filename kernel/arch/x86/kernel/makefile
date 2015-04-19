# Copyright 2013-2014 by Exploerer Developer.
# made by цвсм<2@GhostBirdOS.org> BeatMicrosoft<BeatMicrosoft@GhostBirdOS.org> Lab Explorer<1@GhostBirdOS.org>
# Explorer makefile
# version:Beta
# 2/14/2015 11:40 PM

CFLAGS = -I "../../../include/"
ASMFLAGS = -I "../../../include/"
	
all:
	cd task && make
	nasm -f elf _start.asm -o _start.o $(ASMFLAGS)
	gcc -c cmos.c -o cmos.o $(CFLAGS)
	gcc -c debug.c -o debug.o $(CFLAGS)
	gcc -c do_page_fault.c -o do_page_fault.o $(CFLAGS)
	gcc -c i8254.c -o i8254.o $(CFLAGS)
	gcc -c i8259.c -o i8259.o $(CFLAGS)
	gcc -c kmalloc.c -o kmalloc.o $(CFLAGS)
	gcc -c memory.c -o memory.o $(CFLAGS)
	gcc -c shell.c -o shell.o $(CFLAGS)
	gcc -c time.c -o time.o $(CFLAGS)