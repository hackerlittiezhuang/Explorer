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
#include "unit.h"


/**��Ԫ��Ϣ������ָ������*/
void (*unit_handle[NUM_UNIT_HANDLE])(struct layer *target, unsigned long x, unsigned long y, void *ptr) =
{
	unit_close_handle,
	unit_mini_handle,
	unit_botton_handle
};

/**��ť�ؼ��ṹ����*/
struct unit_botton
{
	char *text;
};

/**��ť��Ϣ������*/
void unit_botton_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr)
{
	window_print(target->winptr, "botton:(%d,%d)", x, y);
}

/**�½���ť����*/
struct layer_unit *unit_new_botton(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width, char *text)
{
	/**��������*/
	correct_para(target, &x, &y, &length, &width);
	
	/**�½����ڵ�Ԫ*/
	GUI_new_unit(target->layer, UNIT_BOTTON, x, y, length, width);
	
	/**���ư�ť*/
	GUI_put_square(target->layer, 0xff0066ff, x, y, length, width);
	
	/**��ʾ��ť���������*/
	GUI_put_string(target->layer, 0xffffffff, x, y + 4, x + length, y + width, font("Standard Font"), text);
}

/**����رհ�ť������*/
void unit_close_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr)
{
	/**���ȸı䴰�ڽṹ��ʶ*/
	target->winptr->ident = 0;
	/**�ͷ���Ӧ����*/
	GUI_free_window(target->winptr);
}

/**������С����ť������*/
void unit_mini_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr)
{
	window_print(target->winptr, "click mini:(%d,%d)", x, y);
}

/**�����µ�Ԫ����*/
struct layer_unit *GUI_new_unit(struct layer *target, unsigned int type, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	struct layer_unit *new_unit;
	
	/**����һ���ڴ�ռ�����µĵ�Ԫ*/
	for (new_unit = NULL; new_unit == NULL; )
		new_unit = kmalloc(sizeof(struct layer_unit), 0);
	
	/**������ֵ*/
	new_unit->x = x;
	new_unit->y = y;
	new_unit->length = length;
	new_unit->width = width;
	new_unit->type = type;
	
	/**����ͼ���е�Ԫ��������*/
	new_unit->next = target->layer_unit_list;
	target->layer_unit_list = new_unit;
}

/**��ȡ�µ�Ԫλ��*/