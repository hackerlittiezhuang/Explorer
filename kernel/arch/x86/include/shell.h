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
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

/**格式化字符串缓冲大小*/
#define		SIZE_OF_STRBUFFER 256

#define		MAX_OUTPUT_SIZE 2048
#if MAX_OUTPUT_SIZE < 80*25
#error "The size of OUTPUT is too small."
#endif

#define		SHELL_MAX_LENGTH 1024
#define		SHELL_MAX_WIDTH 768


/**
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * shell框架结构体
 * 用于提供确定整体shell的框架的信息
 */
struct shell_frame
{
	unsigned long max_x, max_y;
	unsigned long length, width;
	bool refresh_title_flag, refresh_window_flag, refresh_task_bar_flag, refresh_date_flag;
	struct shell_window *map_window;
};

struct shell_window
{
	char *title;
	char output[MAX_OUTPUT_SIZE];
	unsigned int title_backdrop_color;
	unsigned int output_backdrop_color;
	unsigned int output_font_color;
	unsigned long times;
	unsigned long write_point, map_top, map_bottom;
};

//屏幕宽度
extern u32 xsize;
extern u32 ysize;

void inti_shell(void);
void print_mem(unsigned int *point, size_t size);
int printk(const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

unsigned long Jump_line(struct shell_window *window, unsigned long point, long jmp_line);
void Write_to_output(struct shell_window *window, const char *buf);

void refresh_date(void);
void refresh_task_bar(void);
void refresh_title(void);
void refresh_window(void);
void refresh_shell(void);

#endif