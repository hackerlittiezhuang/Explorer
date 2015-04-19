/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * made by Rize<heseri@ghostbirdos.org>
 * Explorer kernel Shell
 * Explorer/arch/x86/kernel/shell_frame.c
 * version:Alpha
 * 7/5/2014 7:08 PM
 */

#include <main.h>
#include "shell.h"
#include <stdbool.h>
#include <stdarg.h>
#include <lib/graphics.h>

/**shell的光标坐标*/
static unsigned long cursor_x = 0, cursor_y = 0;

/**shell的长度、宽度(字节算)*/
static unsigned long length, width;

/**启用标志*/
static bool enable = true;

//open the shell
void enable_shell(void)
{
	/**清空屏幕*/
	clrscr();
	
	/**计算屏幕长度、宽度*/
	length = 800 / 8;
	width = 600 / 16;
	
	/**打开shell*/
	enable = true;
}

//close the shell
void disable_shell(void)
{
	//do not clean the screen
	
	enable = false;
}

/*Clear Screen*/
void clrscr(void)
{
	unsigned char *p = Video_Info.vram;
	unsigned int i;
	for (i=0;i<=SCR_W*SCR_H*BYTEPERPIXEL;i++)
	{
		p[i] = 0;
	}
	
	cursor_x = 0;
	cursor_y = 0;
}

/*put_char: Put a ascii char on screen.
 *Example: put_char('A'); put_char('1');
 *And put a smile:put_char(1);(Smile's ascii number is 1)
*/
void put_char(char ascii)
{
	/**如果shell当前不可用，就直接返回*/
	if (enable == false) return;
	
	/**对控制字符的判断*/
	if (ascii < 0x20)
	{
		/**如果输入字符是换行符，则进行换行*/
		if(ascii == 0xA)
		{
			cursor_x = 0;
			cursor_y ++;
			
			/**如果光标超过边界，滚屏*/
			if (cursor_y >= width)
			{
				roll_screen();
			}
		}
		
		/**返回*/
		return;
	}
	
	/**在光标处位置显示字*/
	draw_font(cursor_x * FONT_W, cursor_y * FONT_H, FGCOLOR, font(DEFAULT_FONT), ascii);
	
	/**将光标移动到下一个位置*/
	cursor_x ++;
	
	if (cursor_x >= length)
	{
		cursor_x = 0;
		cursor_y ++;
	}
	
	if (cursor_y >= width)
	{
		roll_screen();
	}
}

/* Roll Screen */
void roll_screen(void)
{
	unsigned long x, y;
	
	/**循环将第一行之外的其他行向上提一行*/
	for (y = 16; y < 600; y ++)
	{
		for (x = 0; x < 800; x ++)
		{
			putpixel(x, y - 16, getpixel(x, y));
		}
	}
	
	/**未结束*/
	for (y = (600 - ((600 % 16) + 16)); y < 600; y ++)
	{
		for (x = 0; x < 800; x ++)
		{
			putpixel(x, y, BGCOLOR);
		}
	}
	
	cursor_y --;
}

/* printk , made by Hu Wenjie */
int printk(const char *fmt, ...)
{
	char buffer[256];
	va_list arg;
	unsigned long n;
	
	/**format*/
	va_start(arg, fmt);/*init argument point*/
	vsprintf(buffer, fmt, arg);/*write format & get length*/
	
	/**循环将字写入屏幕*/
	for (n = 0; buffer[n] != 0; n ++) put_char(buffer[n]);
	
	/**防止空指针引发危险*/
	va_end(arg);
	
	/**正常返回*/
	return n;
}