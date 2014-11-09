/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Rose<roselyu@163.com>
 * First APP in Explorer
 * Explorer/apps/first.c
 * 2/11/2014 18:06
 */
#include <stddef.h>
#include <lib/string.h>
		void printk(const char* format, ...);
		void _start(void);
void _start(void)
{
//White Screen Demo by Rose
	/*int *p;
	int position_now;
	for (position_now=0;position_now<=800*600*3;position_now++)
	{
		p = position_now + 0xe0000000;
		*p = 0xff;
	}*/
	printk("shabia ");
	return;
}