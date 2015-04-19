# Copyright 2013-2014 by Exploerer Developer.
# made by цвсм<2@GhostBirdOS.org> BeatMicrosoft<BeatMicrosoft@GhostBirdOS.org> Lab Explorer<1@GhostBirdOS.org>
# Explorer makefile
# version:Beta
# 2/14/2015 11:40 PM

CFLAGS = -I "../include/"

all:
	cd window && make
	gcc -c function.c -o function.o $(CFLAGS)
	gcc -c gui_api.c -o gui_api.o $(CFLAGS)
	gcc -c init.c -o init.o $(CFLAGS)
	gcc -c manage.c -o manage.o $(CFLAGS)
	gcc -c refresh.c -o refresh.o $(CFLAGS)