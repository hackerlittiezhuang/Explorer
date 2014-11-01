/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer kernel Shell
 * Explorer/arch/x86/kernel/shell.c
 * version:Alpha
 * 7/5/2014 7:08 PM
 */

#include "../include/shell.h"
#include <arch.h>
#include <lib/graphics.h>
#include <lib/fonts/font.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <types.h>

unsigned char *font;

/**格式化字符串缓冲区*/
char string_buffer[SIZE_OF_STRBUFFER];

static struct shell_frame shell;
static struct shell_window *main_window;

void init_shell(void)
{
	/**获得字库信息*/
	font = get_font_addr("Standard Font");
	/**映射主窗口*/
	shell.map_window = main_window;
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
	main_window->title = "Hello,world!";
	main_window->title_backdrop_color = 0xff0000;
	main_window->output_font_color = 0xffffff;
	main_window->output_backdrop_color = 0x333333;
	main_window->write_point = 0;
	main_window->map_top = 0;
	main_window->map_bottom = 0;
	/**准备好了后允许刷新*/
	shell.refresh_title_flag = true;
	shell.refresh_window_flag = true;
	shell.refresh_task_bar_flag = true;
	shell.refresh_deta_flag = true;
	/**设置定时器，用于刷新界面*/
	settimer(&refresh_shell, 100, 1);
}

/*写入输出缓存函数*/
void Write_to_output(struct shell_window *window, const char *buf)
{

	unsigned long point = (*window).write_point;
	char *output = (*window).output;
	
	/*对参数进行判断*/
	if (buf == NULL) return;
	
	/*循环写入缓冲区*/
	do
	{
		if (point == MAX_OUTPUT_SIZE) point = 0;
		output[point] = *buf;
		point ++;
		buf ++;
	}while (*buf != 0x00);
	(*window).write_point = point;
	(*window).map_bottom = point;
	if (shell.map_window == window)
	{
		shell.refresh_window_flag = true;
	}
}

int printk(const char *fmt, ...)
{
	va_list arg;
	int n;
	va_start(arg, fmt);/*初始化参数指针*/
	n = vsprintf(string_buffer, fmt, arg);/*格式化写入缓存并返回长度*/
	Write_to_output(main_window, string_buffer);
	va_end(arg);/*处理指针，防止误操作*/
	return n;
}

void refresh_deta(void)
{
	/**23:30PM 10/18/2014格式*/
	rectangle((shell.length - (18 * 8)), (shell.width - 16), (18 * 8), 16, 0x808080);
	outtextxy((shell.length - (18 * 8)), (shell.width - 16), 0xffffff, "01:50AM 10/18/2014");
	shell.refresh_deta_flag = false;
	return;
}

void refresh_task_bar(void)
{
	rectangle(0, (shell.width - 16), (shell.length - (18 * 8)), 16, 0x8080ff);
	shell.refresh_task_bar_flag = false;
}

void refresh_title(void)
{
	rectangle(0, 0, shell.length, 16, (*shell.map_window).title_backdrop_color);
	/**如果标题是空指针，就提出警告并返回*/
	if ((*shell.map_window).title == NULL)
	{
		outtextxy(0, 0, 0xffff00, "No title");
		return;
	}
	outtextxy(0, 0, 0xffffff, (*shell.map_window).title);
	shell.refresh_title_flag = false;
	return;
}

void refresh_window(void)
{
	/**重绘背景*/
	rectangle(0, 16, shell.length, (shell.width - (2 * 16)), (*shell.map_window).output_backdrop_color);
	/**重绘内容*/
	unsigned long n;
	unsigned char *output = (*shell.map_window).output;
	unsigned int output_font_color = (*shell.map_window).output_font_color;
	unsigned x = 0, y = 16;
	for (n = (*shell.map_window).map_top; n < (*shell.map_window).map_bottom; n++)
	{
		if (output[n] == 0x00)
		{
		}else if (output[n] == 0x0a)
		{
			y += 16;
			x = 0;
		}else{
			draw_font(x, y, output_font_color, font, output[n]);
			x += 8;
			if (x == shell.length)
			{
				y += 16;
				x = 0;
			}
		}
	}
	shell.refresh_window_flag = false;
}

void refresh_shell(void)
{
	if (shell.refresh_title_flag == true) refresh_title();
	if (shell.refresh_window_flag == true) refresh_window();
	if (shell.refresh_task_bar_flag == true) refresh_task_bar();
	if (shell.refresh_deta_flag == true) refresh_deta();
}