/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer Old Kernel Shell
 * Explorer 0.01/shell/shell.c
 * version:Alpha
 * 7/5/2014 7:08 PM
 */
 
#include "../include/shell.h"
#include <stdarg.h>
#include <types.h>
#include <lib\fonts\font.h>

struct shell{
	u32 x;
	u32 y;
	u32 width;
	u32 height;
	u32 cursor;
	u32 size;
	u32 color;
}shell;
struct CMOS
   {
      unsigned char current_second;
      unsigned char alarm_second;
      unsigned char current_minute;
      unsigned char alarm_minute;
      unsigned char current_hour;
      unsigned char alarm_hour;
      unsigned char current_day_of_week;
      unsigned char current_day;
      unsigned char current_month;
      unsigned char current_year;
      unsigned char status_registers[4];
      unsigned char diagnostic_status;
      unsigned char shutdown_code;
      unsigned char drive_types;
      unsigned char reserved_x;
      unsigned char disk_1_type;
      unsigned char reserved;
      unsigned char equipment;
      unsigned char lo_mem_base;
      unsigned char hi_mem_base;
      unsigned char hi_exp_base;
      unsigned char lo_exp_base;
      unsigned char fdisk_0_type;
      unsigned char fdisk_1_type;
      unsigned char reserved_2[19];
      unsigned char hi_check_sum;
      unsigned char lo_check_sum;
      unsigned char lo_actual_exp;
      unsigned char hi_actual_exp;
      unsigned char century;
      unsigned char information;
      unsigned char reserved3[12];
} cmos;

//static struct shell_frame shell_frame;

void init_shell(void)
{
	/*定时*/
	//settimer(&refresh_shell, 100, 0);
	font=get_font_addr("Standard Font");
	/**初始化框架*/
	/*shell_frame.length = xsize;
	shell_frame.width = ysize;*/
	/*initialize virtual text mode*/
	shell.width = xsize / 8;
	shell.height = ysize / 16;
	shell.x = (xsize - (shell.width * 8))/2;
	shell.y = (ysize - (shell.height * 16))/2;
	shell.cursor = 0;
	shell.size = shell.width * shell.height;
	shell.color = 0xffffffff;
	cmos_info();
	second();
}
void cmos_info(void)
{
	char i;
	char *pointer;
	char byte;
	pointer = (char *) &cmos;
	for (i = 0; i < 0x34; i++)
	{
		io_out8(0x70, i);
		byte = io_in8(0x71);
		*pointer++ = byte;
	}
	printk("  CMOS infomation.\n");
	printk(" >> Current date: %X/%X/%X", cmos.current_month,
		cmos.current_day, cmos.century);
	if(cmos.current_year<10)
	printk("0%X.\n",cmos.current_year);
	else
	printk("%X.\n",cmos.current_year);
	printk(" >> System time: %X:%X:%X.\n", cmos.current_hour,
		cmos.current_minute, cmos.current_second);
	printk(" >> Shutdown type: %X.\n", cmos.shutdown_code);
	printk(" >> Hard disk type %X\n", cmos.fdisk_0_type);
}
void second(void)
{
	char i;
	char *pointer;
	char byte;
	pointer = (char *) &cmos;
	for (i = 0; i < 0x34; i++)
	{
		io_out8(0x70, i);
		byte = io_in8(0x71);
		*pointer++ = byte;
	}
	printk(" >> Current time: %X:%X:%X.\n", cmos.current_hour,
		cmos.current_minute, cmos.current_second);
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
