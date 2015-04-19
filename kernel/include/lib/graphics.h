/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphics Library Definition
 * Explorer/include/lib/graphics.h
 * version:Alpha
 * 11/1/2014 12:09 AM
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <lib/fonts/font.h>

struct Video_Info{
	unsigned char *vram;
	unsigned long xres, yres;
	unsigned long vram_length;
	unsigned long bit_per_pixel;
}Video_Info;

void Inti_Graph(void);
void outtextxy(unsigned long x, unsigned long y, unsigned int color, unsigned char *string);
void draw_font(unsigned long x, unsigned long y, unsigned int color, struct font_info *font_info, unsigned char ascii);
void rectangle(unsigned long x, unsigned long y, unsigned long height, unsigned long width, unsigned int color);
void line(unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1, unsigned int color);
unsigned int getpixel(unsigned int x, unsigned int y);
void putpixel(unsigned int x, unsigned int y, unsigned int color);
void clear_screen(void);

#endif