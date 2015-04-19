/**
 * Copyright 2013-2014 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer kernel shell head
 * ExplorerOS/Kernel/include/shell.h
 * version:Alpha
 * 7/5/2014 7:08 PM
 */

#ifndef SHELL_H_
#define SHELL_H_

#include "types.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

/**��ʽ���ַ��������С*/
#define		SIZE_OF_STRBUFFER 256

typedef struct shell_window* shell;

/**�������*/
struct Frame_Attribute{
	unsigned long x, y;
	unsigned long length, width;
};

/**shell��ܽṹ�壬�����ṩȷ������shell�Ŀ�ܵ���Ϣ*/
struct shell_frame{
	unsigned long length, width;/*����shell�ĳ��Ϳ�*/
	struct Frame_Attribute title;
	struct Frame_Attribute output;
	struct Frame_Attribute task_bar;
	bool refresh_title_flag, refresh_window_flag, refresh_task_bar_flag, refresh_date_flag;
	struct shell_window *map_window;
};

struct shell_window{
	char *title;
	char *output;
	unsigned int title_backdrop_color;
	unsigned int output_backdrop_color;
	unsigned int output_font_color;
	unsigned long cursor, buffer_start, map;
};

void inti_shell(void);
struct shell_window *Alloc_Shell_Window(void);
void print_mem(unsigned int *point, size_t size);
int printk(const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

void Shell_Output_Char(struct shell_window *window, char word);
long Init_Shell_Window(struct shell_window *window, char *title);

unsigned long Jump_line(shell window, unsigned long point, long jmp_line);
void Write_To_Output(struct shell_window *window, const char *buf);

void set_back_color(shell window, u32 color);

void refresh_date(void);
void date(void);
void refresh_task_bar(void);
void refresh_title(void);
void refresh_window(void);
int refresh_shell(void *arg);

#endif