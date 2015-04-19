/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com> Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer PS/2 Mouse Support
 * Explorer/drivers/mouse.c
 * version:Alpha
 * 8/26/2014 10:30 AM
 */

#include <mouse.h>
#include <keyboard.h>
#include<arch.h>
#include<lib/graphics.h>
#include <GUI.h>
#include "i8042.h"

/** A FIFO buffer for mouse*/
static char mouse_buffer[SIZE_OF_BUFFER_MOUSE];
static unsigned long write_point, read_point;

/**description of mouse x&y*/
unsigned char mouse_cmd;
static long mouse_x, mouse_y;

/**save mouse info temporary*/
static char mouse_info[3];
static unsigned long mouse_info_point;

void init_mouse(void)
{
	/**initialize mouse_info_point zero*/
	mouse_info_point = 0;
	
	/**set the location of mouse in centre of screen*/
	mouse_x = (Video_Info.xres / 2);
	mouse_y = (Video_Info.yres / 2);
	
	/**register to 8259*/
	register_PIC(12, &int_mouse_handle, "Mouse");
	
	/**allow mouse send info to CPU*/
	cmd_mouse(0xF4);
	/**open mouse*/
	enable_mouse();
}

void put_mouse_x(long x)
{
	mouse_x = x;
}

long get_mouse_x(void)
{
	return mouse_x;
}

void put_mouse_y(long y)
{
	mouse_y = y;
}

long get_mouse_y(void)
{
	return mouse_y;
}

void int_mouse_handle(void)
{
	mouse_info[mouse_info_point] = read_8042();
	if (mouse_info_point == 2)
	{
		/**重新指向数组开始处*/
		mouse_info_point = 0;
		
		/**处理信息*/
		mouse_cmd = mouse_info[0];
		mouse_x += mouse_info[1];
		mouse_y += -(mouse_info[2]);
		
		GUI_test();
	}else{
		mouse_info_point ++;
	}
	EOI();
}