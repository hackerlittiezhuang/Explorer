/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Error Output System
 * Explorer/arch/x86/kernel/error.c
 * version:Alpha
 * 12/6/2014 7:15 PM
 */

#include <error.h>
#include <lib\graphics.h>
#include <info.h>

void error(char *info)
{
	/**禁止中断*/
	io_cli();
	/**display error background*/
	rectangle(	0, 0,
				Video_Info.xres, Video_Info.yres, ERROR_BAKEGROUND_COLOR);
	
	/**put information of kernel*/
	outtextxy(0, 0, ERROR_FOREGROUND_COLOR, KERNEL_NAME);
	
	/**put error information*/
	outtextxy(50, 200, ERROR_FOREGROUND_COLOR, info);
	
	/**halt*/
	for (;;);
}