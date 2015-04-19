/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Initialization
 * Explorer/GUI/window.c
 * version:Alpha
 * 8/16/2014 9:08 AM
 */

#include <lib/fonts/font.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <GUI.h>
#include <memory.h>
#include <arch.h>
#include <lib/graphics.h>
#include "window.h"
#include "../recourse.h"
#include "../layer.h"
#include <mouse.h>

/**
 * 图层顺序：
 * 根图层位于最顶端
 * 光标图层位于次顶端
 * 任务栏图层位于次次顶端
 * 最新创建的窗口位于其他窗口顶部，任务栏图层的底部
 * 任何窗口都在这里
 * 背景图层位于最底端
 */

/**背景图层和光标图层*/
struct layer *background_layer = NULL, *pointer_layer = NULL, *taskbar_layer = NULL;

/**测试窗口*/
struct window *new_window, *new_window_2;

/**测试函数：消息的分发*/
void GUI_test(void)
{
	extern unsigned char mouse_cmd;
	unsigned long x, y;
	struct layer *layer;
	
	/**如果这次事件非点击事件，则放弃*/
	if (mouse_cmd != 0x9) return;

	/**获取坐标信息*/
	x = get_mouse_x();
	y = get_mouse_y();
	
	/**从最顶层窗口开始向下搜索*/
	for (layer = taskbar_layer->bottom; layer->winptr != NULL; layer = layer->bottom)
	{
		/**进行区域判断*/
		if ((x >= layer->x) & (x < layer->x + layer->length) & (y >= layer->y) & (y < layer->y + layer->width))
		{
			/**设置被点击的窗口为活动窗口*/
			window_set_active(layer->winptr);
			
			/**输出信息到相应窗口*/
			window_print(layer->winptr, "Clicked in (%d,%d).", x - layer->x, y - layer->y);
			
			/**退出循环*/
			break;
		}
	}
}

/**初始化窗口系统*/
void init_Window(void)
{
	/**初始化背景*/
	init_background();
	
	/**初始化光标*/
	init_pointer();
	
	/**初始化任务栏*/
	init_taskbar();
	
	/**窗口测试*/
	new_window = GUI_Window("Ghost Bird OS", WINDOW_NORMAL, 350, 1, 200, 320);
	new_window_2 = GUI_Window("Ghost Bird OS", WINDOW_NORMAL, 590, 1, 200, 320);
	
}

/**初始化背景函数*/
void init_background(void)
{
	/*initialization picture layer*/
	for (background_layer = NULL; background_layer == NULL;)
	{
		background_layer = GUI_new_layer(0, 0, Video_Info.xres, Video_Info.yres);
	}
	
	/**相关参数设置*/
	background_layer->visiable = true;
	
	/**显示字符串*/
	GUI_put_string(background_layer, 0xffffff00, 100, 50, 0, 0, font("Standard Font"), "Explorer Graphical User Interface");
	GUI_put_string(background_layer, 0xffff0000, 100, 66, 0, 0, font("Standard Font"), "Copyright 2013-2015 by Lab Explorers. All rights reserved.");
	GUI_put_string(background_layer, 0xff2020ff, 100, 82, 0, 0, font("Standard Font"), "Initialize for desktop...");

	/**填充灰色*/
	GUI_put_square(background_layer, 0xffa0a0a0, 0, 0, 800, 600);
	//GUI_put_square(background_layer, 0x80ffffff, 0, 0, 800, 600);
	
	/**加载图片*/
	//file_open("BACKDROP.PIC", (*background_layer).buf);//BACKDROP_PIC_point
	
	/**刷新界面*/
	//GUI_refresh();
}

/**刷新光标函数*/
void refresh_pointer(void)
{
	long _xmouse, _ymouse;
		_xmouse = get_mouse_x();
		if (_xmouse < 0)
		{
			_xmouse = (0);
			put_mouse_x(_xmouse);
		}else if (_xmouse >= Video_Info.xres)
		{
			_xmouse = (Video_Info.xres - 1);
			put_mouse_x(_xmouse);
		}
		
		_ymouse = get_mouse_y();
		if (_ymouse < 0)
		{
			_ymouse = (0);
			put_mouse_y(_ymouse);
		}if (_ymouse >= Video_Info.yres)
		{
			_ymouse = (Video_Info.yres - 1);
			put_mouse_y(_ymouse);
		}
		GUI_set_position(pointer_layer, _xmouse, _ymouse);
}

/**初始化光标函数*/
void init_pointer(void)
{
	/**创建个新图层*/
	for (pointer_layer = NULL; pointer_layer == NULL;)
		pointer_layer = GUI_new_layer(0, 0, 12, 20);
	
	/**从图层链表中脱离出来*/
	pointer_layer->top->bottom = pointer_layer->bottom;
	pointer_layer->bottom->top = pointer_layer->top;
	
	/**设定好光标的层级位于根图层之下其他图层之上*/
	pointer_layer->bottom = layer_root->bottom;
	pointer_layer->top = layer_root;
	layer_root->bottom = pointer_layer;
	pointer_layer->bottom->top = pointer_layer;
	
	/**将光标点阵图映射到图层上*/
	GUI_map(pointer_layer, Pointer_Bitmap, 12, 20);
	
	/**使图层可以看到*/
	pointer_layer->visiable = true;
	
	/**设置刷新*/
	settimer(&refresh_pointer, 1000 / 50, 0);
}

/**
 * 初始化任务栏函数
 * 时钟格式：10:49 2/4/2015
 */
void init_taskbar(void)
{
	/**初始化任务栏图层*/
	for (taskbar_layer = NULL; taskbar_layer == NULL;)
		taskbar_layer = GUI_new_layer(0, Video_Info.yres - 20, Video_Info.xres, 20);
	
	/**将任务栏图层从图层链表中脱离出来*/
	taskbar_layer->top->bottom = taskbar_layer->bottom;
	taskbar_layer->bottom->top = taskbar_layer->top;
	
	/**将任务栏置于光标图层之下，其他图层之上*/
	taskbar_layer->top = pointer_layer;
	taskbar_layer->bottom = pointer_layer->bottom;
	pointer_layer->bottom = taskbar_layer;
	taskbar_layer->bottom->top = taskbar_layer;
	
	/**使任务栏可见*/
	(*taskbar_layer).visiable = true;
	
	
	/**绘制任务栏*/
	GUI_put_square(taskbar_layer, 0xc0ffffff, 0, 0, Video_Info.xres, 20);			/**任务区*/
	GUI_line(taskbar_layer, 0xeeffffff, 0, 0, Video_Info.xres, 0);					/**边际线*/
	
	/**显示时钟*/
	settimer(&window_refresh_date, 500, 0);
}

/**活动窗口指针*/
static struct window *window_active = NULL;

/**设置活动窗口函数*/
void window_set_active(struct window *target)
{
	/**如果活动窗口就是当前窗口*/
	if (window_active == target)
	{
		/**直接返回*/
		return;
		
	/**判断是否已经有活动窗口*/
	}else if (window_active != NULL)
	{
		/**有则需要将当前活动窗口设置为非活动窗口*/
		
	/**如果当前还没有活动窗口*/
	}else{
		
		/**设置当前窗口为活动窗口*/
		window_active = target;
		
		/**绘制活动窗口边框*/
		
	}
}