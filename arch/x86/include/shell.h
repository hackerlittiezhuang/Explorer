/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by 迷彩红星<1@GhostBirdOS.org>
 *Explorer 内核shell平台支持代码头
 *ExplorerOS/Kernel/include/shell.h
 *version:Alpha
 *7/5/2014 7:08 PM
 */

#ifndef SHELL
#define SHELL

#include "types.h"

//字库
extern u8 font[256*16];
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