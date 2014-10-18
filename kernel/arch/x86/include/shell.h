/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer kernel shell head
 * ExplorerOS/Kernel/include/shell.h
 * version:Alpha
 * 7/5/2014 7:08 PM
 */

#ifndef SHELL
#define SHELL

#include "types.h"
#include <stdbool.h>

#define		SHELL_MAX_LENGTH 1024
#define		SHELL_MAX_WIDTH 768

/**
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * shell框架结构体
 * 用于提供确定整体shell的框架的信息
 */
struct shell_frame
{
	unsigned long length, width;
	bool refresh_title_flag, refresh_window_flag, refresh_task_bar_flag, refresh_deta_flag;
	struct shell_window *map_window;
};

struct shell_window
{
	unsigned char *title;
	unsigned char output[2048];
	unsigned long write_point;
};

//字库
extern u8 font[256*16];
//屏幕宽度
extern u32 xsize;
extern u32 ysize;

void inti_shell(void);
int printk(const char *fmt, ...);
void debug(u32 *address, u32 size);
void color(u32 color);
void put_font(u8 ascii);
void put_string(unsigned long x, unsigned long y, unsigned int color, unsigned char *string);
void draw_font(unsigned long x, unsigned long y, unsigned int color, unsigned char ascii);
void draw_square(unsigned long x, unsigned long y, unsigned long width, unsigned long height, unsigned int color);

void refresh_deta(void);
void refresh_task_bar(void);
void refresh_title(void);
void refresh_window(void);
void refresh_shell(void);

#endif