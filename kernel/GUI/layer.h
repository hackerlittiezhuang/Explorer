/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Layer Definition
 * Explorer/GUI/layer.h
 * version:Alpha
 * 2/10/2015 1:38 PM
 */

#ifndef GUI_LAYER_H_
#define GUI_LAYER_H_

#include "window/window.h"
#include <stdlib.h>

/**图层结构体*/
struct layer
{
	bool visiable;
	long x, y;
	unsigned long length, width;
	unsigned int *buf;
	
	/**图层双向链表*/
	struct layer *top, *bottom;
	
	/**指向相应窗口的指针*/
	struct window *winptr;
};

/**根图层*/
extern struct layer *layer_root;

/**背景图层和光标图层*/
extern struct layer *background_layer, *pointer_layer, *taskbar_layer;

#define MAX_LAYERS	256
struct layer_control
{
	unsigned int length, width, top;
	unsigned int *buffer;
	struct layer layer[MAX_LAYERS];
};

#endif
