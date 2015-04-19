/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Library
 * Explorer/GUI/window/lib.c
 * version:Alpha
 * 2/5/2015 1:37 PM
 */

#include <lib/graphics.h>
#include <GUI.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>
#include "window.h"
#include "../layer.h"

/**
 * Explorer内核图形化界面窗口系统库
 * 在GUI的图层化基础上的高级图形库
 */

/**创建Window函数
 * 创建一个窗口，标题为title，
 * 风格为style，
 * 当x = 0且y = 0时，窗口会创建在最中间
 * 窗口中有效内容尺寸为length*width
 */
struct window *GUI_Window(char *title, char style, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	unsigned long real_length, real_width;
	/**根据不同的风格，进行不同的初始化*/
	if (style == WINDOW_NONE)
	{
		/**无边框*/
		real_length = length;
		real_width = width;
	}else if (style == WINDOW_NORMAL)
	{
		/**算进边框大小*/
		real_length = length + 2;
		real_width = width + 33;
	}
	
	/**分配一个新的图层*/
	struct window *new_window;
	new_window = kmalloc(sizeof(struct window), 0);
	if (new_window == NULL) error("window create failed.");
	
	/**结构体赋值*/
	new_window->title = title;
	new_window->style = style;
	new_window->length = length;
	new_window->width = width;
	new_window->print_x = 0;
	new_window->print_y = 0;
	new_window->ident = WINDOW_IDENT;
	
	/**确定新窗口的位置
	 * 如果x,y都为0，则默认新窗口位于正中间
	 * 如果x,y至少有一个不为0，则新窗口位置在(x,y)
	 */
	if ((x == 0) & (y == 0))
	{
		x = ((Video_Info.xres - real_length) / 2);
		y = ((Video_Info.yres - real_width) /2);
	}
	
	/**创建、初始化图层*/
	new_window->layer = GUI_new_layer(x, y, real_length, real_width);
	
	/**将该图层从图层链表中脱离*/
	new_window->layer->top->bottom = new_window->layer->bottom;
	new_window->layer->bottom->top = new_window->layer->top;
	
	/**将该图层置于任务栏之下，其他窗口之上*/
	new_window->layer->top = taskbar_layer;
	new_window->layer->bottom = taskbar_layer->bottom;
	taskbar_layer->bottom = new_window->layer;
	new_window->layer->bottom->top = new_window->layer;
	
	/**设置为可见*/
	new_window->layer->visiable = true;
	
	/**设置指针*/
	new_window->layer->winptr = new_window;
	
	/**判断风格是否是无风格*/
	if (style == WINDOW_NONE)
	{
		/**无边框*/
		GUI_put_square(new_window->layer, 0xffffffff, 0, 0, length, width);
		
	/**判断是不是正常风格*/
	}else if (style == WINDOW_NORMAL)
	{
		/**绘制窗体边框*/
		GUI_put_square(new_window->layer, 0xE50066FF, 1, 0, length, 32);
		
		/**左右边框*/
		GUI_put_square(new_window->layer, 0xE50066FF, 0, 0, 1, real_width);
		GUI_put_square(new_window->layer, 0xE50066FF, real_length - 1, 0, 1, real_width);
		
		/**下边框*/
		GUI_put_square(new_window->layer, 0xE50066FF, 1, real_width - 1, length, 1);
		
		/**主体*/
		GUI_put_square(new_window->layer, 0xffffffff, 1, 32, length, width);
		
		/**窗体标题*/
		GUI_put_string(new_window->layer, 0xffffffff, 5, 8, 0, 0, font("simsun"), title);
	}

	/**返回新窗口的指针*/
	return new_window;
}

/**纠正绘制参数函数
 * 窗体有无边框、边框风格不同时
 * 绘制到图层上的实际地址是不一样的
 */
void correct_para(struct window *target, unsigned long *x, unsigned long *y, unsigned long *length, unsigned long *width)
{
	/**根据窗口风格纠正参数*/
	if (target->style == WINDOW_NONE)
	{
		/**无边框*/
	}else if (target->style == WINDOW_NORMAL)
	{
		/**有窗体边框*/
		*x += 1;
		*y += 32;
		*length = (target->length + 1);
		*width = (target->width + 32);
	}
	return;
}

/**输出字符串到窗口*/
void window_string(struct window *target, unsigned long x, unsigned long y, char *s)
{
	unsigned long length, width;
	/**纠正参数*/
	correct_para(target, &x, &y, &length, &width);

	/**输出字符串*/
	GUI_put_string(target->layer, 0xff000000, x, y, length, width, font("Standard Font"), s);
}

/**窗口信息输出函数*/
int window_print(struct window *target, const char *fmt, ...)
{
	va_list arg;
	unsigned int n;
	
	/**缓冲区*/
	char buffer[64];
	
	/**初始化参数指针*/
	va_start(arg, fmt);
	
	/**格式化写入缓存并返回长度*/
	n = vsprintf(buffer, fmt, arg);
	
	/**输出字符串到窗口*/
	window_string(target, target->print_x, target->print_y, buffer);
	
	/**指针指向新区域*/
	target->print_y += 16;
	
	va_end(arg);/*处理指针，防止误操作*/
	return n;
}