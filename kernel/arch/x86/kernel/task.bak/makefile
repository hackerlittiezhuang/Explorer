# Copyright 2013-2014 by Exploerer Developer.
# made by цвсм<2@GhostBirdOS.org> BeatMicrosoft<BeatMicrosoft@GhostBirdOS.org> Lab Explorer<1@GhostBirdOS.org>
# Explorer makefile
# version:Beta
# 2/14/2015 11:40 PM

CFLAGS = -I "../../../../include/"
	
all:
	nasm -f elf init_stack.asm -o init_stack.o
	nasm -f elf switch_to.asm -o switch_to.o
	gcc -c task.c -o task.o $(CFLAGS)
	