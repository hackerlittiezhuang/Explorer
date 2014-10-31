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
//�ֿ�
unsigned char *font;
//��Ļ���
extern u32 xsize;
extern u32 ysize;

void inti_shell(void);
/*���������ʱ�û�����*/
int printk(const char *fmt, ...);
void debug(u32 *address, u32 size);
void color(u32 color);
void put_font(u8 ascii);
void draw_font(u32 x, u32 y, u32 color, u8 ascii);
void draw_square(u32 x, u32 y, u32 width, u32 height, u32 color);

#endif