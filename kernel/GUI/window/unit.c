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

/**按钮控件结构描述*/
struct unit_botton
{
	char *text;
};

/**新建按钮函数*/
struct window_unit *unit_new_botton(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	/**纠正参数*/
	correct_para(target, &x, &y, &length, &width);
	
	/**新建窗口单元*/
	window_new_unit(target, x, y, length, width);
	
	/**绘制按钮*/
	GUI_put_square(target->layer, 0xffff0000, x, y, length, width);
}


/**创建新单元函数*/
struct window_unit *window_new_unit(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	struct window_unit *new_unit;
	
	/**申请一块内存空间放下新的单元*/
	for (new_unit = NULL; new_unit == NULL; )
		new_unit = kmalloc(sizeof(struct window_unit), 0);
	
	/**纠正参数*/
	correct_para(target, &x, &y, NULL, NULL);
	
	/**参数赋值*/
	new_unit->x = x;
	new_unit->y = y;
	new_unit->length = length;
	new_unit->width = width;
	new_unit->type = 0;
	
	/**加入窗体中单元单向链表*/
	target->window_unit_list = new_unit;
	new_unit->next = target->window_unit_list;
}

/**获取新单元位置*/