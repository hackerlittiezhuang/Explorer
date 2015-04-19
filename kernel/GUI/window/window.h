/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window Management Definition
 * Explorer/GUI/window.h
 * version:Alpha
 * 12/14/2014
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "../layer.h"

/**任务栏颜色*/
#define TASKBAR_COLOR		0xa0000000
#define TASKBAR_LINE_COLOR	0xff000000

/**时钟字符串颜色*/
#define TIME_COLOR			0xffffffff

/**单元结构*/
struct window_unit
{
	unsigned int type;				/**单元的种类*/
	unsigned long x, y;				/**单元的位置*/
	unsigned long length, width;	/**单元的长宽*/
	struct window_unit *next;		/**下一个单元结构*/
};

/**GUI控制台窗口*/
struct window *GUI_control;

/**窗口结构体辨识特殊数*/
#define WINDOW_IDENT	11262

/**
 * 窗口的风格有以下种类
 * 无边框窗口：没有边框，在这种情况下，标题将会被忽略
 * 正常窗口：拥有正常的窗口边框
 */
#define WINDOW_NONE		0				//无边框
#define WINDOW_NORMAL	1				//正常边框

/**窗口结构体*/
struct window{
	short int ident;						/**窗口结构体辨识*/
	char *title;							/**窗口标题*/
	char style;								/**窗口的风格*/
	struct layer *layer;					/**窗口所对应的图层*/
	unsigned long length, width;			/**窗口的长、宽*/
	unsigned long print_x, print_y;			/**打印字符串的坐标*/
	struct window_unit *window_unit_list;	/**窗体单元单向链表*/
};

/**加载图片函数*/
struct GUI_image *window_load_image(char *filename);

/**新建按钮函数*/
struct window_unit *unit_new_botton(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width);

/**创建新单元函数*/
struct window_unit *window_new_unit(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width);


/**初始化窗口系统*/
void init_Window(void);

/**创建Window函数
 * 创建一个窗口，标题为title，
 * 风格为style，
 * 当x = 0且y = 0时，窗口会创建在最中间
 * 窗口中有效内容尺寸为length*width
 */
struct window *GUI_Window(char *title, char style, unsigned long x, unsigned long y, unsigned long length, unsigned long width);

/**纠正绘制参数函数
 * 窗体有无边框、边框风格不同时
 * 绘制到图层上的实际地址是不一样的
 */
void correct_para(struct window *target, unsigned long *x, unsigned long *y, unsigned long *length, unsigned long *width);

/**输出字符串到窗口*/
void window_string(struct window *target, unsigned long x, unsigned long y, char *s);

/**窗口信息输出函数*/
int window_print(struct window *target, const char *fmt, ...);

/**加载图片线程
 * filename是要加载的背景文件
 */
int load_background(void *filename);

/**初始化背景函数*/
void init_background(void);

/**刷新光标函数*/
void refresh_pointer(void);

/**初始化光标函数*/
void init_pointer(void);

/**初始化任务栏函数*/
void init_taskbar(void);

/**设置活动窗口函数*/
void window_set_active(struct window *target);

/**window系统时钟刷新函数*/
void window_refresh_date(void);


#endif
