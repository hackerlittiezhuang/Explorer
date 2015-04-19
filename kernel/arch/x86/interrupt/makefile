# Copyright 2013-2014 by Exploerer Developer.
# made by цвсм<2@GhostBirdOS.org> BeatMicrosoft<BeatMicrosoft@GhostBirdOS.org> Lab Explorer<1@GhostBirdOS.org>
# Explorer makefile
# version:Beta
# 2/14/2015 11:40 PM

CFLAGS = -I "../../../include/"

all:
	gcc -c handle.c -o handle.o $(CFLAGS)
	gcc -c interrupt.c -o interrupt.o $(CFLAGS)
	nasm -f elf entry.asm -o entry.o
	