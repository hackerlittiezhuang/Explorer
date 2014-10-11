/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer Graphical User Interface head
 *ExplorerOS/Kernel/include/GUI.h
 *version:Alpha
 *8/16/2014 4:59 PM
 */

#ifndef GUI_H_
#define GUI_H_

#include "stdbool.h"

#define MAX_LAYERS	32
#define MAX_WINDOW	256

struct struct_layer
{
	bool visiable;
	unsigned int offset_x;
	unsigned int offset_y;
	unsigned int length, width;
	unsigned int *buf;
	unsigned int flags;
	struct layer_control *parent_control;
};

struct layer_control
{
	unsigned int length, width, top;
	unsigned int *buffer;
	struct struct_layer layer[MAX_LAYERS];
	struct struct_layer *parent_layer;
};

struct layer_control layer_root;

void init_GUI(void);
struct struct_layer *GUI_layer_alloc(struct layer_control *layer_control);
long int GUI_set_position(struct struct_layer *layer, unsigned long x, unsigned long y);
long int GUI_set_resolution(struct struct_layer *layer, unsigned long length, unsigned long width);
long int GUI_refresh(struct layer_control *layer_control);
long int GUI_put_square(struct struct_layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long length, unsigned long width);
long int GUI_put_pix(struct struct_layer *layer, unsigned int color, unsigned long x, unsigned long y);
unsigned int GUI_get_pix(struct struct_layer *layer, unsigned long x, unsigned long y);

typedef unsigned long int handle;

struct struct_window
{
	handle handle;
	unsigned long length, width;
	unsigned char title[64];
};

handle GUI_creat_Window(unsigned long x, unsigned long y, unsigned char *title);

#endif