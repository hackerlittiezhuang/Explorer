/**
 * Copyright 2013-2014 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer kernel Shell
 * Explorer/arch/x86/kernel/shell_frame.c
 * version:Alpha
 * 7/5/2014 7:08 PM
 */

#include "shell.h"
#include <arch.h>
#include <memory.h>
#include <lib/mem.h>
#include <lib/string.h>
#include <lib/graphics.h>
#include <lib/fonts/font.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <task.h>
#include <types.h>
#include "cmos.h"

struct font_info *shell_font;

/**格式化字符串缓冲区*/
char string_buffer[SIZE_OF_STRBUFFER];

/**定义主显示窗口*/
shell main_window;

/**定义shell框架*/
static struct shell_frame shell_frame;

/**定义固定缓冲分配长度*/
unsigned long buffer_size;

/**刷新线程*/
static task_id shell_refresh;

void init_shell(void)
{
	/**获得字库信息*/
	shell_font = font("Standard Font");
	/**shell框架的初始化*/
	shell_frame.length = Video_Info.xres;/*shell的长度*/
	shell_frame.width = Video_Info.yres;/*shell的宽度*/
	shell_frame.title.length = Video_Info.xres;/*title的长度*/
	shell_frame.title.width = 16;/*title的宽度*/
	shell_frame.task_bar.length = Video_Info.xres;/*task bar的长度*/
	shell_frame.task_bar.width = 16;/*task bar的宽度*/
	shell_frame.output.length = Video_Info.xres;/*output的长度*/
	shell_frame.output.width = shell_frame.width - (shell_frame.task_bar.width + shell_frame.title.width);/*output的宽度*/
	
	shell_frame.title.x = 0;/*title的x*/
	shell_frame.title.y = 0;/*title的y*/
	shell_frame.output.x = 0;/*output的x*/
	shell_frame.output.y = shell_frame.title.x + shell_frame.title.width;/*output的y*/
	shell_frame.task_bar.x = 0;/*task bar的x*/
	shell_frame.task_bar.y = shell_frame.output.y + shell_frame.output.width;/*task bar的y*/
	
	/**设置缓冲分配长度*/
	buffer_size = ((shell_frame.output.length / 8) * (shell_frame.output.width / 16) * 8);
	
	/**对主窗口进行分配和初始化*/
	main_window = Alloc_Shell_Window();
	Init_Shell_Window(main_window, "New shell.");
	
	/**首次刷新*/
	shell_frame.refresh_title_flag = true;
	shell_frame.refresh_window_flag = true;
	shell_frame.refresh_task_bar_flag = true;
	shell_frame.refresh_date_flag = true;
	
	/**设线程刷新*/
	/*for (shell_refresh = NULL; shell_refresh == NULL;)
	{
		shell_refresh = new_task(&refresh_shell, NULL);
	}
	settimer(&refresh_date, 1000, 0);*/
	
	printk("Hello,world\n");
	//printk("value is %d", Jump_line(main_window, 0, 3));
	//for (;;) printk("key words:%#x.\n", get_key());
}

void Shell_Output_Char(struct shell_window *window, char word)
{
	if (word == 0x0a)
	{
		window->cursor += (shell_frame.output.length / 8) - (window->cursor % (shell_frame.output.length / 8));
	}else{
		(*window).output[window->cursor] = word;
		(*window).cursor ++;
	}
	
	if (window->cursor >= buffer_size) window->cursor = 0;
	
	if ((window->cursor % (shell_frame.output.length / 8)) == 0)
	{
		memset(&((*window).output[window->cursor]), 0, (shell_frame.output.length / 8));
	}
	/*返回*/
	return;
}

/**
 * 分配shell窗口函数
 * 入口参数：output_line_size:缓存区列长度
 * 返回参数：新shell结构体的首地址
 */
struct shell_window *Alloc_Shell_Window(void)
{
	struct shell_window *new_window;
	char *output;
	
	/*为新的shell分配内存空间*/
	new_window = kmalloc(sizeof(struct shell_window), 0);
	//new_window = vmalloc(sizeof(struct shell_window));
	/*分配失败返回NULL，为了减少耦合性，增加这个重赋值过程*/
	if (new_window == NULL) error("shell error!");
	
	/*为新的shell分配输出缓存*/
	output = vmalloc(buffer_size);/*行数*每行字数*单位大小得出实际缓存大小*/
	if (output == NULL)
	{
		kfree(new_window);/*分配失败，得释放已经获得的内存*/
		return NULL;
	}
	/*用memset函数将新的结构体和缓存区给清0，减少出错的几率*/
	memset(new_window, 0, sizeof(struct shell_window));/*清空结构体*/
	memset(output, 0, buffer_size);/*清空缓存区*/
	/*对shell结构体进行必要的赋值*/
	new_window->output = output;
	
	return new_window;
}

long Init_Shell_Window(struct shell_window *window, char *title)
{
	/*进行常规的初始化*/
	window->title = title;
	window->title_backdrop_color = 0xff0000;
	window->output_font_color = 0xffffff;
	window->output_backdrop_color = 0x333333;
	window->buffer_start = 0;
	window->cursor = window->buffer_start;
	window->map = window->cursor;
	/*设定映射*/
	shell_frame.map_window = window;
	return 0;
}

/**
 * 写入输出缓存函数
 */
void Write_To_Output(struct shell_window *window, const char *buf)
{
	/*对参数进行判断*/
	if (buf == NULL) return;
	
	/*循环写入缓冲区*/
	for (; *buf != 0x00; buf ++) Shell_Output_Char(window, *buf);

	/*滚屏*/
	if (((window->cursor - window->map) / (shell_frame.output.length / 8)) >= (shell_frame.output.width / 16))
	{
		window->map += (shell_frame.output.length / 8);
		if (window->map > buffer_size) window->map = 0;
	}
	
	/*设定刷新标志*/
	if (shell_frame.map_window == window)
	{
		shell_frame.refresh_window_flag = true;
		shell_frame.refresh_task_bar_flag = true;
	}
}

/**
 * 打印内存函数
 * 将point指向的size长度的内存数据值打印在屏幕上
 */
void print_mem(unsigned int *point, size_t size)
{
	unsigned n;
	for (n = 0; (n < size / sizeof(unsigned int)); n++)
	{
		printk("%#010x,", point[n]);
	}
	printk("\nmemory:%#x-%#x,   size:%d.\n", point, ((char *)point + size), size);
}

/**标准输入输出函数*/
int printk(const char *fmt, ...)
{
	va_list arg;
	int n;
	va_start(arg, fmt);/*初始化参数指针*/
	n = vsprintf(string_buffer, fmt, arg);/*格式化写入缓存并返回长度*/
	Write_To_Output(main_window, string_buffer);
	va_end(arg);/*处理指针，防止误操作*/
	
	/**唤醒刷新线程*/
	wakeup(shell_refresh);
	
	/**正常返回*/
	return n;
}

void set_back_color(shell window, u32 color)
{
	(*window).output_backdrop_color = color;
	if (shell_frame.map_window == window)
	{
		shell_frame.refresh_window_flag = true;
		refresh_shell(NULL);
	}
}

/**
 * 以下是界面刷新的相关函数
 */
void refresh_date(void)
{
	/**23:30PM 10/18/2014格式*/
	struct CMOS cmos;
	//strlen
	shell_frame.task_bar.length = Video_Info.xres - 19*8;/*task bar的长度*/
	rectangle((shell_frame.length - (19 * 8)), (shell_frame.width - 16), (19 * 8), 16, 0x808080);
	//取得时间
	char i;
	char *pointer;
	char byte;
	pointer = (char *) &cmos;
	for (i = 0; i < 0x34; i++)
	{
       io_out8(0x70, i);
       byte = io_in8(0x71);
       *pointer++ = byte;
	}
	//时间字符串格式化
	char timebuf[64];
	
	sprintf(timebuf,"%02X:%02X:%02X %X/%X/%02X%02x", cmos.current_hour, cmos.current_minute, cmos.current_second, cmos.current_month, cmos.current_day, cmos.century, cmos.current_year);
	outtextxy((shell_frame.length - (19 * 8)), (shell_frame.width - 16), 0xffffff, timebuf);
	//shell_frame.refresh_date_flag = false;
	return;
}

/**刷新任务栏*/
void refresh_task_bar(void)
{
	rectangle(shell_frame.task_bar.x, shell_frame.task_bar.y, shell_frame.task_bar.length, shell_frame.task_bar.width, 0x8080ff);
	shell_frame.refresh_task_bar_flag = false;
}

/**刷新标题*/
void refresh_title(void)
{
	/**画矩形*/
	rectangle(shell_frame.title.x, shell_frame.title.y, shell_frame.title.length, shell_frame.title.width, (*shell_frame.map_window).title_backdrop_color);
	
	/**如果标题是空指针，就提出警告并返回*/
	if ((*shell_frame.map_window).title == NULL) (*shell_frame.map_window).title = "No title";
	outtextxy(0, 0, 0xffffff, (*shell_frame.map_window).title);
	shell_frame.refresh_title_flag = false;
	return;
}

void refresh_window(void)
{
	/**重绘背景*/
	rectangle(0, 16, shell_frame.length, (shell_frame.width - (2 * 16)), (*shell_frame.map_window).output_backdrop_color);
	/**重绘内容*/
	char *output = (*shell_frame.map_window).output;
	unsigned long x, y = shell_frame.output.y;
	unsigned long length = shell_frame.output.length, width = shell_frame.output.width;
	unsigned long output_font_color = (*shell_frame.map_window).output_font_color, n;
	
	for (n = (*shell_frame.map_window).map; y < (shell_frame.output.y + width - 16); y += 16)
	{
		for (x = shell_frame.output.x; x < (shell_frame.output.x + length); x += 8)
		{
			draw_font(x, y, output_font_color, shell_font, output[n]);
			n ++;
		}
		if (n == buffer_size) n = 0;
	}
finish:
	shell_frame.refresh_window_flag = false;/*刷新实现*/
}

int refresh_shell(void *arg)
{
	for (;;)
	{
		/**进行刷新*/
		if (shell_frame.refresh_title_flag == true) refresh_title();
		if (shell_frame.refresh_window_flag == true) refresh_window();
		if (shell_frame.refresh_task_bar_flag == true) refresh_task_bar();
		
		/**刷新完后挂起*/
		sleep();
	}
}