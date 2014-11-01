/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer Graphics Library
 * Explorer/lib/graphics.c
 * version:Alpha
 * 11/1/2014 11:56 AM
 */

#include <video.h>
#include <lib/fonts/font.h>
#include <lib/graphics.h>

void outtextxy(unsigned long x, unsigned long y, unsigned int color, unsigned char *string)
{
	unsigned long point;
	/**获得字库信息*/
	unsigned char *font;
	font = get_font_addr("Standard Font");
	for (point = 0; string[point] != 0x00; point ++)
	{
		draw_font(x, y, color, font, string[point]);
		x += 8;
	}
}

/*显示字*/
void draw_font(unsigned long x, unsigned long y, unsigned int color, unsigned char *font, unsigned char ascii)
{
	unsigned long p, i, font_offset;/*字库偏移量*/
	unsigned char d;
	font_offset = ascii * 16;
	for (i = 0; i < 16; i++)
	{
		d = font[font_offset + i];
		if ((d & 0x80) != 0) { putpixel(x, y + i, color); }
		if ((d & 0x40) != 0) { putpixel(x + 1, y + i, color); }
		if ((d & 0x20) != 0) { putpixel(x + 2, y + i, color); }
		if ((d & 0x10) != 0) { putpixel(x + 3, y + i, color); }
		if ((d & 0x08) != 0) { putpixel(x + 4, y + i, color); }
		if ((d & 0x04) != 0) { putpixel(x + 5, y + i, color); }
		if ((d & 0x02) != 0) { putpixel(x + 6, y + i, color); }
		if ((d & 0x01) != 0) { putpixel(x + 7, y + i, color); }
	}
}

void rectangle(unsigned long x, unsigned long y, unsigned long height, unsigned long width, unsigned int color)
{
	unsigned long m, n;
	for (n = 0; n != width; n ++)
	{
		for (m = 0; m != height; m ++)
		{
			putpixel(x + m, y + n, color);
		}
	}
}

unsigned int getpixel(unsigned int x, unsigned int y)
{
	unsigned int i;
	if (x < xsize & y < ysize)
	{
		i = ((y * xsize) + x)*3;
		return (vram[i] + (vram[i+1] << 8) + (vram[i+2] << 16));
	}
}

void putpixel(unsigned int x, unsigned int y, unsigned int color)
{
	int i;
	if (x < xsize & y < ysize)
	{
		i = ((y * xsize) + x)*3;
		vram[i] = color;
		vram[i+1] = color >> 8;
		vram[i+2] = color >> 16;
	}
}