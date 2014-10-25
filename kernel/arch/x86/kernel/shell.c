/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer kernel Shell
 * Explorer/arch/x86/kernel/shell.c
 * version:Alpha
 * 7/5/2014 7:08 PM
 */

#include "../include/shell.h"
#include <lib/fonts/font.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <types.h>

unsigned char *font;

/**格式化字符串缓冲区*/
char string_buffer[SIZE_OF_STRBUFFER];

static struct shell_frame shell;
static struct shell_window main_window;

void init_shell(void)
{
	/**获得字库信息*/
	font = get_font_addr("linux_sun");
	/**映射主窗口*/
	shell.map_window = &main_window;
	/**
	 * 设定shell的大小，一般将跟窗口一样大。
	 * 如果窗口比设计最大值还要大，就设置为设计最大值。
	 */
	if (xsize <= SHELL_MAX_LENGTH)
	{
		shell.length = xsize;
	}else{
		shell.length = SHELL_MAX_LENGTH;
	}
	if (ysize <= SHELL_MAX_WIDTH)
	{
		shell.width = ysize;
	}else{
		shell.width = SHELL_MAX_WIDTH;
	}
	/**主窗口的设定*/
	main_window.title = "Hello,world!";
	main_window.output_backdrop_color = 0x333333;
	main_window.write_point = 0;
	/**准备好了后允许刷新*/
	shell.refresh_title_flag = true;
	shell.refresh_window_flag = true;
	shell.refresh_task_bar_flag = true;
	shell.refresh_deta_flag = true;
	/**设置定时器，用于刷新界面*/
	settimer(&refresh_shell, 100, 0);
	
	printk("Hello,world!%s","This is string.");
}

void put_font(u8 ascii)
{
}

int printk(const char *fmt, ...)
{
	va_list arg;
	int n;
	va_start(arg,fmt);/*初始化参数指针*/
	n = _vsnprintf(string_buffer, SIZE_OF_STRBUFFER, fmt, arg);/*格式化写入缓存并返回长度*/
	va_end(arg);/*处理指针，防止误操作*/
	return n;
}

int _vsnprintf(char* str, size_t size, const char* fmt, va_list arg)
{
	do
	{
		if (*fmt == 0x00)
		{
			fin:goto fin;
		}
		(*shell.map_window).output[(*shell.map_window).write_point] = *fmt;
		(*shell.map_window).write_point ++;
	}while (*fmt++);
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
	/**如果标题是空指针，就提出警告并返回*/
	if ((*shell.map_window).title == NULL)
	{
		put_string(0, 0, 0xffff00, "No title!");
		return;
	}
	put_string(0, 0, 0xffffff, (*shell.map_window).title);
	shell.refresh_title_flag = false;
	return;
}

void refresh_window(void)
{
	draw_square(0, 16, shell.length, (shell.width - (2 * 16)), (*shell.map_window).output_backdrop_color);
	
	put_string(0, 16, 0xffffff, (*shell.map_window).output);
	shell.refresh_window_flag = false;
}

void refresh_shell(void)
{
	if (shell.refresh_title_flag == true) refresh_title();
	if (shell.refresh_window_flag == true) refresh_window();
	if (shell.refresh_task_bar_flag == true) refresh_task_bar();
	if (shell.refresh_deta_flag == true) refresh_deta();
}