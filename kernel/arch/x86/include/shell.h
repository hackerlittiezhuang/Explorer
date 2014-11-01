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
	unsigned long length, width;
	bool refresh_title_flag, refresh_window_flag, refresh_task_bar_flag, refresh_deta_flag;
	struct shell_window *map_window;
};

struct shell_window
{
	char *title;
	char output[MAX_OUTPUT_SIZE];
	unsigned int title_backdrop_color;
	unsigned int output_backdrop_color;
	unsigned int output_font_color;
	unsigned long write_point, map_top, map_bottom;
};
void cmos_info(void);
void second(void);
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
//屏幕宽度
extern u32 xsize;
extern u32 ysize;

void inti_shell(void);
int printk(const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
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