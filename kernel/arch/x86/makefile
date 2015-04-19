# Copyright 2013-2014 by Exploerer Developer.
# made by цвсм<2@GhostBirdOS.org> BeatMicrosoft<BeatMicrosoft@GhostBirdOS.org> Lab Explorer<1@GhostBirdOS.org>
# Explorer makefile
# version:Beta
# 2/14/2015 11:40 PM

CFLAGS = -I "../../include/"

all:
	cd CPU && make
	cd interrupt && make
	cd kernel && make
	gcc -c Architecture.c -o Architecture.o $(CFLAGS)
	nasm -f elf io.asm -o io.o
	nasm -f elf syscalls.asm -o syscalls.o
	