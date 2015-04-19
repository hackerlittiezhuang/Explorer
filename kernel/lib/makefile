# Copyright 2013-2014 by Exploerer Developer.
# made by цвсм<2@GhostBirdOS.org> BeatMicrosoft<BeatMicrosoft@GhostBirdOS.org> Lab Explorer<1@GhostBirdOS.org>
# Explorer lib makefile
# version:Beta
# 2/14/2015 11:40 PM

CFLAGS = -I "../include/"

all:
	cd fonts && make
	gcc -c mem.c -o mem.o $(CFLAGS)
	gcc -c string.c -o string.o $(CFLAGS)
	gcc -c graphics.c -o graphics.o $(CFLAGS)
	gcc -c math.c -o math.o $(CFLAGS)
	gcc -c vsprintf.c -o vsprintf.o $(CFLAGS)