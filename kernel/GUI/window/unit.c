/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Unit
 * Explorer/GUI/window/unit.c
 * version:Alpha
 * 2/15/2015 7:58 PM
 */

#include <GUI.h>
#include "memory.h"
#include "window.h"

/**��ť�ؼ��ṹ����*/
struct unit_botton
{
	char *text;
};

/**�½���ť����*/
struct window_unit *unit_new_botton(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	/**��������*/
	correct_para(target, &x, &y, &length, &width);
	
	/**�½����ڵ�Ԫ*/
	window_new_unit(target, x, y, length, width);
	
	/**���ư�ť*/
	GUI_put_square(target->layer, 0xffff0000, x, y, length, width);
}


/**�����µ�Ԫ����*/
struct window_unit *window_new_unit(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	struct window_unit *new_unit;
	
	/**����һ���ڴ�ռ�����µĵ�Ԫ*/
	for (new_unit = NULL; new_unit == NULL; )
		new_unit = kmalloc(sizeof(struct window_unit), 0);
	
	/**��������*/
	correct_para(target, &x, &y, NULL, NULL);
	
	/**������ֵ*/
	new_unit->x = x;
	new_unit->y = y;
	new_unit->length = length;
	new_unit->width = width;
	new_unit->type = 0;
	
	/**���봰���е�Ԫ��������*/
	target->window_unit_list = new_unit;
	new_unit->next = target->window_unit_list;
}

/**��ȡ�µ�Ԫλ��*/