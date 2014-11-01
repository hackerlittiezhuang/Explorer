/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer kernel Shell
 * Explorer 0.01/shell/shell.c
 * version:Alpha
 * 7/5/2014 7:08 PM
 */
 
#include "../include/shell.h"
#include <stdarg.h>
#include <types.h>

struct shell{
	u32 x;
	u32 y;
	u32 width;
	u32 height;
	u32 cursor;
	u32 size;
	u32 color;
}shell;


static struct shell_frame shell_frame;

void init_shell(void)
{
	
	/*定时*/
	settimer(&refresh_shell, 100, 0);
	/**初始化框架*/
	shell_frame.length = xsize;
	shell_frame.width = ysize;
	/*initialize virtual text mode*/
	shell.width = xsize / 8;
	shell.height = ysize / 16;
	shell.x = (xsize - (shell.width * 8))/2;
	shell.y = (ysize - (shell.height * 16))/2;
	shell.cursor = 0;
	shell.size = shell.width * shell.height;
	shell.color = 0xffffffff;
}

int printk(const char *fmt, ...)
{
	/**
	 *variable parameter
	 *va_list\va_start\va_arg\va_end all in Explorer/include
	 */
	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	char *ch1;
	unsigned int int_val;
	do
	{
		if (*fmt == '%')
		{
			fmt++;
			switch(*fmt)
			{
				case 'd':
				break;
				/*hex to string*/
				case 'X':
					int_val = va_arg(arg_ptr, unsigned int);
					char shr;
					for (shr = 28; shr >= 0; shr -= 4)
					{
						if ((((int_val) >> shr) & 0xf) <= 9)
						{
							put_font((((int_val) >> shr) & 0xf) + 0x30);
						}else{
							put_font((((int_val) >> shr) & 0xf) + 0x37);
						}
					}
				break;
				
				/*output string*/
				case 's':
					ch1 = va_arg(arg_ptr, char *);
					for (; *ch1 != 0x00; ch1++)
					{
						put_font(*ch1);
					}
				break;
				/*output char "%"*/
				case '%':
					put_font('%');
				break;
				/*nothing*/
				default:
					put_font(*fmt);
				break;
			}
		}else{
			put_font(*fmt);
		}
	}while (*fmt++);
	va_end(arg_ptr);
}

void debug(u32 *address, u32 size)
{
	printk("debug:from 0x%X to 0x%X is:\n", address, address+size);
	for (; size > 0; size -= 4)
	{
		printk("%X ",*address);
		address ++;
	}
	printk("\n");
	return;
}

/*向上滚屏功能函数*/
void scr_up(void)
{
	u32 x,y;
	for (y = shell.y; y < (shell.y + ((shell.height - 1) * 16)); y ++)
	{
		for (x = shell.x; x < (shell.x + (shell.width * 8)); x ++)
		{
			put_pix_24(x, y, get_pix_24(x, (y + 16)));
			put_pix_24(x, (y + 16), 0x00000000);
		}
	}
	shell.cursor -= shell.width;
	return;
}

/*设置颜色*/
void color(u32 color)
{
	shell.color = color;
}

/*输出字*/
void put_font(u8 ascii)
{
	/*换行键的判断*/
	if ((ascii == 0x0a))
	{
		shell.cursor -= (shell.cursor % shell.width);
		shell.cursor += shell.width;
	}

	/*对是否需要滚屏判断*/
	if (shell.cursor >= shell.size) {
		scr_up();
	}
	if (ascii < 0x20)/*对控制字符的判断*/
	{
		return;
	}
	/*由模拟文本模式参数到实际图形模式的转换*/
	u32 x, y;
	x = shell.x + (shell.cursor % shell.width) * 8;
	y = shell.y + (shell.cursor / shell.width) * 16;
	/*调用显示函数*/
	draw_font(x, y, shell.color, ascii);
	/*模拟光标指向下一个单位*/
	shell.cursor ++;
}

put_string(u32 x, u32 y, u32 color, u8 *string)
{
	u32 point;
	for (point = 0; string[point] != 0x00; point ++)
	{
		draw_font(x, y, color, string[point]);
		x += 8;
	}
}

/*显示字*/
void draw_font(u32 x, u32 y, u32 color, u8 ascii)
{
	u32 p, i, font_offset;/*字库偏移量*/
	u8 d;
	font_offset = ascii * 16;
	for (i = 0; i < 16; i++)
	{
		d = font[font_offset + i];
		if ((d & 0x80) != 0) { put_pix_24(x, y + i, color); }
		if ((d & 0x40) != 0) { put_pix_24(x + 1, y + i, color); }
		if ((d & 0x20) != 0) { put_pix_24(x + 2, y + i, color); }
		if ((d & 0x10) != 0) { put_pix_24(x + 3, y + i, color); }
		if ((d & 0x08) != 0) { put_pix_24(x + 4, y + i, color); }
		if ((d & 0x04) != 0) { put_pix_24(x + 5, y + i, color); }
		if ((d & 0x02) != 0) { put_pix_24(x + 6, y + i, color); }
		if ((d & 0x01) != 0) { put_pix_24(x + 7, y + i, color); }
	}
}

void draw_square(u32 x, u32 y, u32 width, u32 height, u32 color)
{
	u32 m, n;
	for (n = 0; n != height; n ++)
	{
		for (m = 0; m != width; m ++)
		{
			put_pix_24(x + m, y + n, color);
		}
	}
}

void refresh deta(void)
{
}

void refresh_task_bar(void)
{
}

void refresh_title(void)
{
}

void refresh_window(void)
{
}

void refresh_shell(void)
{
}
