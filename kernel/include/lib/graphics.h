/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer Graphics Library define
 * Explorer/include/lib/graphics.h
 * version:Alpha
 * 11/1/2014 12:09 AM
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

void outtextxy(unsigned long x, unsigned long y, unsigned int color, unsigned char *string);
void draw_font(unsigned long x, unsigned long y, unsigned int color, unsigned char *font, unsigned char ascii);
void rectangle(unsigned long x, unsigned long y, unsigned long height, unsigned long width, unsigned int color);
unsigned int getpixel(unsigned int x, unsigned int y);
void putpixel(unsigned int x, unsigned int y, unsigned int color);

#endif