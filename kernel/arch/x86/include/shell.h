/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Rose<roselyu@163.com>
 *Explorer Old Shell Header
 *ExplorerOS/Kernel/include/shell.h
 *version:Alpha
 *19/10/2014
 */

#ifndef SHELL
#define SHELL

#include "types.h"
void cmos_info(void);
void second(void);
//字库
unsigned char *font;
//屏幕宽度
extern u32 xsize;
extern u32 ysize;

void inti_shell(void);
/*这个函数临时用汇编完成*/
int printk(const char *fmt, ...);
void debug(u32 *address, u32 size);
void color(u32 color);
void put_font(u8 ascii);
void draw_font(u32 x, u32 y, u32 color, u8 ascii);
void draw_square(u32 x, u32 y, u32 width, u32 height, u32 color);

#endif