/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Library
 * Explorer/GUI/window/lib.c
 * version:Alpha
 * 2/5/2015 1:37 PM
 */

#include <lib/string.h>
#include <lib/graphics.h>
#include <GUI.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>
#include "window.h"
#include "unit.h"
#include "../layer.h"
#include "../recourse.h"

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
struct window *GUI_window(char *title, char style, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
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
		real_length = length + (WINDOW_NORMAL_FRAME_WIDTH * 2);
		real_width = width + WINDOW_NORMAL_HEADER_WIDTH + WINDOW_NORMAL_FRAME_WIDTH;
		
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
	strncpy(new_window->title, title, WINDOW_NUM_TITLE);
	new_window->style = style;
	new_window->length = length;
	new_window->width = width;
	new_window->print_x = 0;
	new_window->print_y = 0;
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
		
		/**���ư�ť*/
		GUI_map(new_window->layer, close_f_botton, new_window->layer->length - close_f_botton->length - WINDOW_NORMAL_FRAME_WIDTH, 0, 0, 0, 0);
		GUI_map(new_window->layer, mini_f_botton, new_window->layer->length - (close_f_botton->length + mini_f_botton->length) - WINDOW_NORMAL_FRAME_WIDTH, 0, 0, 0, 0);
		
		// /**������Ԫ*/
		GUI_new_unit(new_window->layer, UNIT_CLOSE, new_window->layer->length - close_f_botton->length - WINDOW_NORMAL_FRAME_WIDTH, 0, close_f_botton->length, close_f_botton->width);
		GUI_new_unit(new_window->layer, UNIT_MINI, new_window->layer->length - (close_f_botton->length + mini_f_botton->length) - WINDOW_NORMAL_FRAME_WIDTH, 0, mini_f_botton->length, mini_f_botton->width);
		
		/**����*/
		GUI_put_square(new_window->layer, 0xffffffff, WINDOW_NORMAL_FRAME_WIDTH, WINDOW_NORMAL_HEADER_WIDTH, length, width);
	}

	/**�����´��ڵ�ָ��*/
	return new_window;
}

/**�ͷŴ��ں���*/
void GUI_free_window(struct window *target)
{
	/**�ͷŴ���ʹ�õ�ͼ����Դ*/
	GUI_free_layer(target->layer);
	
	/**�ͷŴ���*/
	kfree(target);
	
	/**��������*/
	return;
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
		*x += WINDOW_NORMAL_FRAME_WIDTH;
		*y += WINDOW_NORMAL_HEADER_WIDTH;
		
	/**�ж��Ƿ񳬹��߽磬����������Ա߽�Ϊ׼*/
		if ((length != NULL) & (*length >= (target->length + WINDOW_NORMAL_FRAME_WIDTH)))
			*length = target->length + WINDOW_NORMAL_FRAME_WIDTH;
		
		if ((width != NULL) & (*width >= (target->width + WINDOW_NORMAL_HEADER_WIDTH)))
			*width = target->width + WINDOW_NORMAL_HEADER_WIDTH;
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
	
	/**����ָ�룬��ֹ�����*/
	va_end(arg);
	return n;
}