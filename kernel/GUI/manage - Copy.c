/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Management
 * Explorer/GUI/manage.c
 * version:Alpha
 * 8/14/2014 1:59 PM
 */

#include <memory.h>
#include <stdlib.h>
#include <GUI.h>
#include <lib/graphics.h>
#include "layer.h"

/**allocate a layer from the layer_control*/
struct layer *GUI_new_layer(long x, long y, unsigned long length, unsigned long width)
{
	struct layer *new_layer;
	
	/**参数判断*/
	if (length == 0 | width == 0) return NULL;
	
	/**创建图层*/
	unsigned long i = layer_root.top;
	if (i != MAX_LAYERS)
	{
		layer_root.layer[i].flags = 1;
		layer_root.layer[i].visiable = false;
		layer_root.top++;
		new_layer = &layer_root.layer[i];
	}else{
		return NULL;
	}
	
	/**设置图层大小、分辨率*/
	GUI_set_position(new_layer, x, y);
	GUI_set_resolution(new_layer, length, width);
	
	return new_layer;
}

/*change the height of the layer*/
long int GUI_set_height(struct layer *layer, unsigned long int height)
{
	
}

/*change the position of the layer*/
long int GUI_set_position(struct layer *layer, long x, long y)
{
	long old_x = (*layer).x, old_y = (*layer).y;
	(*layer).x = x;
	(*layer).y = y;
	GUI_refresh_block(x, y, (*layer).length, (*layer).width);
	GUI_refresh_block(old_x, old_y, (*layer).length, (*layer).width);
}

/*change the resolution of the layer*/
long int GUI_set_resolution(struct layer *layer, unsigned long length, unsigned long width)
{
	/*If it had buffer,we must free first*/
	if ((*layer).buf != NULL)
	{
		vfree((*layer).buf);
	}
	(*layer).buf = vmalloc(length * width * 4);
	(*layer).length = length;
	(*layer).width = width;
	return 0;
}

long int GUI_map(struct layer *layer, void *target, unsigned long length, unsigned long width)
{
	/*If it had buffer,we must free first*/
	if ((*layer).buf != NULL)
	{
		vfree((*layer).buf);
	}
	
	(*layer).buf = target;
	(*layer).length = length;
	(*layer).width = width;
	
	GUI_refresh_block(layer->x, layer->y, (*layer).length, (*layer).width);
}