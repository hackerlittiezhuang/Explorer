/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer kernel Shell
 * Explorer 0.01/shell/shell.c
 * version:Alpha
 * 7/5/2014 7:08 PM
 */

#include "../include/shell.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <types.h>


static struct shell_frame shell;
static struct shell_window main_window;
unsigned char *font;
void init_shell(void)
{
	font = get_font_addr("Standard Font");
	if (font == NULL)
	{
		err:goto err;
	}
	shell.map_window = &main_window;
	if (xsize <= SHELL_MAX_LENGTH) shell.length = xsize;
	else shell.length = SHELL_MAX_LENGTH;
	if (ysize <= SHELL_MAX_WIDTH) shell.width = ysize;
	else shell.width = SHELL_MAX_WIDTH;
	
	main_window.title = "Hello,world!";
	shell.refresh_title_flag = true;
	shell.refresh_window_flag = true;
	shell.refresh_task_bar_flag = true;
	shell.refresh_deta_flag = true;
	/**设置定时器，用于刷新界面*/
	settimer(&refresh_shell, 100, 0);
}

int printk(const char *fmt, ...)
{
}

void draw_square(unsigned long x, unsigned long y, unsigned long height, unsigned long width, unsigned int color)
{
	unsigned long m, n;
	for (n = 0; n != width; n ++)
	{
		for (m = 0; m != height; m ++)
		{
			put_pix_24(x + m, y + n, color);
		}
	}
}

void put_string(unsigned long x, unsigned long y, unsigned int color, unsigned char *string)
{
	unsigned long point;
	for (point = 0; string[point] != 0x00; point ++)
	{
		draw_font(x, y, color, string[point]);
		x += 8;
	}
}

/*显示字*/
void draw_font(unsigned long x, unsigned long y, unsigned int color, unsigned char ascii)
{
	unsigned long p, i, font_offset;/*字库偏移量*/
	unsigned char d;
	font_offset = ascii * 16;
	for (i = 0; i < 16; i++)
	{
		d = font[font_offset + i];
		if ((d & 0x80) != 0) { put_pix_24(x, y + i, color); }
		if ((d & 0x40) != 0) { put_pix_24(x + 1, y + i, color); }
		if ((d & 0x20) != 0) { put_pix_24(x + 2, y + i, color); }
		if ((d & 0x10) != 0) { put_pix_24(x + 3, y + i, color); }
		if ((d & 0x08) != 0) { put_pix_24(x + 4, y + i, color); }
		if ((d & 0x04) != 0) { put_pix_24(x + 5, y + i, color); }
		if ((d & 0x02) != 0) { put_pix_24(x + 6, y + i, color); }
		if ((d & 0x01) != 0) { put_pix_24(x + 7, y + i, color); }
	}
}

void refresh_deta(void)
{
	/**23:30PM 10/18/2014格式*/
	draw_square((shell.length - (18 * 8)), (shell.width - 16), (18 * 8), 16, 0x808080);
	put_string((shell.length - (18 * 8)), (shell.width - 16), 0xffffff, "01:50AM 10/18/2014");
	shell.refresh_deta_flag = false;
	return;
}

void refresh_task_bar(void)
{
	draw_square(0, (shell.width - 16), (shell.length - (18 * 8)), 16, 0x000066);
	shell.refresh_task_bar_flag = false;
}

void refresh_title(void)
{
	draw_square(0, 0, shell.length, 16, 0xff0000);
	put_string(0, 0, 0xffffff, (*shell.map_window).title);
	shell.refresh_title_flag = false;
	return;
}

void refresh_window(void)
{
	draw_square(0, 16, shell.length, (shell.width - (2 * 16)), 0x333333);
	shell.refresh_window_flag = false;
}

void refresh_shell(void)
{
	if (shell.refresh_title_flag == true) refresh_title();
	if (shell.refresh_window_flag == true) refresh_window();
	if (shell.refresh_task_bar_flag == true) refresh_task_bar();
	if (shell.refresh_deta_flag == true) refresh_deta();
}