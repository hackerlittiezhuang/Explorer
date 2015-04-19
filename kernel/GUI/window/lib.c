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
 * Explorer�ں�ͼ�λ����洰��ϵͳ��
 * ��GUI��ͼ�㻯�����ϵĸ߼�ͼ�ο�
 */

/**����Window����
 * ����һ�����ڣ�����Ϊtitle��
 * ���Ϊstyle��
 * ��x = 0��y = 0ʱ�����ڻᴴ�������м�
 * ��������Ч���ݳߴ�Ϊlength*width
 */
struct window *GUI_Window(char *title, char style, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	unsigned long real_length, real_width;
	/**���ݲ�ͬ�ķ�񣬽��в�ͬ�ĳ�ʼ��*/
	if (style == WINDOW_NONE)
	{
		/**�ޱ߿�*/
		real_length = length;
		real_width = width;
		
		/**�ޱ߿򴰿���С���ó���Ϊ0*/
		if ((length * width) == 0)
		{
			/**���󷵻�*/
			return NULL;
		}
	}else if (style == WINDOW_NORMAL)
	{
		/**����߿��С*/
		real_length = length + 2;
		real_width = width + 33;
		
		/**�����߿򴰿���С���ó���Ϊ0*/
		if ((length * width) == 0)
		{
			/**���󷵻�*/
			return NULL;
		}
	}
	
	/**����һ���µ�ͼ��*/
	struct window *new_window;
	new_window = kmalloc(sizeof(struct window), 0);
	if (new_window == NULL) error("window create failed.");
	
	/**�ṹ�帳ֵ*/
	new_window->title = title;
	new_window->style = style;
	new_window->length = length;
	new_window->width = width;
	new_window->print_x = 0;
	new_window->print_y = 0;
	new_window->window_unit_list = NULL;
	new_window->ident = WINDOW_IDENT;
	
	/**ȷ���´��ڵ�λ��
	 * ���x,y��Ϊ0����Ĭ���´���λ�����м�
	 * ���x,y������һ����Ϊ0�����´���λ����(x,y)
	 */
	if ((x == 0) & (y == 0))
	{
		x = ((Video_Info.xres - real_length) / 2);
		y = ((Video_Info.yres - real_width) /2);
	}
	
	/**��������ʼ��ͼ��*/
	new_window->layer = GUI_new_layer(x, y, real_length, real_width);
	
	/**����ͼ���ͼ������������*/
	new_window->layer->top->bottom = new_window->layer->bottom;
	new_window->layer->bottom->top = new_window->layer->top;
	
	/**����ͼ������������֮�£���������֮��*/
	new_window->layer->top = taskbar_layer;
	new_window->layer->bottom = taskbar_layer->bottom;
	taskbar_layer->bottom = new_window->layer;
	new_window->layer->bottom->top = new_window->layer;
	
	/**����Ϊ�ɼ�*/
	new_window->layer->visiable = true;
	
	/**����ָ��*/
	new_window->layer->winptr = new_window;
	
	/**�жϷ���Ƿ����޷��*/
	if (style == WINDOW_NONE)
	{
		/**�ޱ߿�*/
		GUI_put_square(new_window->layer, 0xffffffff, 0, 0, length, width);
		
	/**�ж��ǲ����������*/
	}else if (style == WINDOW_NORMAL)
	{
		/**�����µĴ���Ϊ�����*/
		window_set_active(new_window);
		
		/**����*/
		GUI_put_square(new_window->layer, 0xffffffff, 1, 32, length, width);
	}

	/**�����´��ڵ�ָ��*/
	return new_window;
}

/**�������Ʋ�������
 * �������ޱ߿򡢱߿���ͬʱ
 * ���Ƶ�ͼ���ϵ�ʵ�ʵ�ַ�ǲ�һ����
 */
void correct_para(struct window *target, unsigned long *x, unsigned long *y, unsigned long *length, unsigned long *width)
{
	/**���ݴ��ڷ���������*/
	if (target->style == WINDOW_NONE)
	{
		/**�ޱ߿�*/
	}else if (target->style == WINDOW_NORMAL)
	{
		/**�д���߿�*/
		*x += 1;
		*y += 32;
		
	/**�ж��Ƿ񳬹��߽磬����������Ա߽�Ϊ׼*/
		if ((length != NULL) & (*length >= (target->length + 1)))
		{
			*length = target->length;
		}
		
		if ((width != NULL) & (*width >= (target->width + 32)))
		{
			*width = target->width;
		}
	}
	return;
}

/**����ַ���������*/
void window_string(struct window *target, unsigned long x, unsigned long y, char *s)
{
	unsigned long length = 0xffffffff, width = 0xffffffff;
	/**��������*/
	correct_para(target, &x, &y, &length, &width);

	/**����ַ���*/
	GUI_put_string(target->layer, 0xff000000, x, y, length, width, font("Standard Font"), s);
}

/**������Ϣ�������*/
int window_print(struct window *target, const char *fmt, ...)
{
	va_list arg;
	unsigned int n;
	
	/**������*/
	char buffer[64];
	
	/**��ʼ������ָ��*/
	va_start(arg, fmt);
	
	/**��ʽ��д�뻺�沢���س���*/
	n = vsprintf(buffer, fmt, arg);
	
	/**����ַ���������*/
	window_string(target, target->print_x, target->print_y, buffer);
	
	/**ָ��ָ��������*/
	target->print_y += 16;
	
	va_end(arg);/*����ָ�룬��ֹ�����*/
	return n;
}