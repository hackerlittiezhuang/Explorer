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
#include <lib/mem.h>
#include <lib/graphics.h>
#include "layer.h"

/**创建新图层函数*/
struct layer *GUI_new_layer(long x, long y, unsigned long length, unsigned long width)
{
	struct layer *new_layer;
	
	/**参数判断*/
	if (length == 0 | width == 0) return NULL;
	
	/**分配内存给图层结构体*/
	for (new_layer = NULL; new_layer == NULL;)
	new_layer = kmalloc(sizeof(struct layer), 0);
	
	/**结构体清空，防止意外参数干扰*/
	memset(new_layer, 0, sizeof(struct layer));
	
	/**加入双向链表中*/
	new_layer->top = layer_root->top;
	new_layer->bottom = layer_root;
	new_layer->top->bottom = new_layer;
	layer_root->top = new_layer;
	
	/**设置图层位置*/
	GUI_set_position(new_layer, x, y);
	
	/**设置图层分尺寸*/
	GUI_set_resolution(new_layer, length, width);
	
	/**正常返回*/
	return new_layer;
}

/**
 * 改变图层高度函数（未正常运行）
 * 将target图层设置为height高度
 * 返回值 = 0：设置无效（已经是该高度）
 * 返回值 = 1：根图层（无意义）
 * 返回值为其他值时，代表正常返回，成功改变
 */
unsigned long GUI_set_height(struct layer *target, unsigned long int height)
{
	unsigned long total = 0, retval;
	struct layer *layer = layer_root;
	
	/**先判断总共有多少个图层*/
	do{
		layer = layer->bottom;
		total ++;
	}while (layer != layer_root);
	
	/**要将target图层设置为height，
	 * 限制height最小值不得小于1
	 * 限制height最大不得超过(total-1)
	 * 最高图层是layer_root图层，高度为total
	 */
	
	if (height < 1) return 0;
	if (height >= total) height = (total - 1);
	
	//height = 5;
	
	/**目标高度不得等于该图层现在的高度*/
	unsigned long n = 1;
	for (layer = layer_root->top; layer != target; layer = layer->top) n ++;
	
	/**判断高度是否重复，如果是则意外返回*/
	if (n == height) return 0;
	//error("asd");
	/**确定了设置的高度*/
	retval = height;
	
	/**将目标高度位置记录下来*/
	struct layer *top, *bottom;
	height --;
	for (layer = layer_root->top; height != 0; layer = layer->top) height --;
	bottom = layer->bottom;
	top = layer;
	
	/**将target图层从双向链表中移除*/
	target->top->bottom = target->bottom;
	target->bottom->top = target->top;
	
	/**将target图层加入到双向链表中新的位置*/
	target->top = top;
	target->bottom = bottom;
	top->bottom = target;
	bottom->top = target;
	
	/**正常返回*/
}

/**获取图层高度函数
 * 返回值 = 0：获取失败（无该图层）
 */
unsigned long GUI_get_height(struct layer *target)
{
	unsigned long retval = 0;
	struct layer *layer;	
	
	/**循环计算*/
	layer = layer_root->top;
	
	do{
		/**计数加一*/
		retval ++;
		
		/**判断是否找到*/
		if (layer == target) return retval;
		
		/**向上搜寻*/
		layer = layer->top;
	/**排除无该图层的情况*/
	}while ((layer != layer_root) | (target == layer_root));
	
	/**运行到这里就是无图层的情况*/
	return 0;
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
	/**如果该图层之前还有缓存，首先要释放该缓存*/
	if (layer->buf != NULL)
	{
		vfree((*layer).buf);
	}
	
	/**分配新的内存*/
	for (layer->buf = NULL; layer->buf == NULL;)
		layer->buf = vmalloc(length * width * 4);
	
	/**修改相关参数*/
	layer->length = length;
	layer->width = width;
	
	/**正常返回*/
	return 0;
}

long int GUI_map(struct layer *layer, void *target, unsigned long length, unsigned long width)
{
	/**如果该图层之前还有缓存，首先要释放该缓存*/
	if ((*layer).buf != NULL)
	{
		vfree((*layer).buf);
	}
	
	/**参数设置*/
	layer->buf = target;
	layer->length = length;
	layer->width = width;
	
	/**对图层块刷新*/
	GUI_refresh_block(layer->x, layer->y, (*layer).length, (*layer).width);
}
