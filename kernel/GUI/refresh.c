/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Refresh
 * Explorer/GUI/refresh.c
 * version:Alpha
 * 12/14/2014 1:59 PM: created
 * 12/22/2014 3:37 PM: previous file code about refresh add to this file
 */

#include <stdlib.h>
#include <GUI.h>
#include <lib/graphics.h>
#include "layer.h"

/**刷新像素函数
 * (x,y)是在屏幕上需要刷新的像素坐标
 * 该函数为通用函数，仅为兼容性考虑。
 * 在具体平台上建议用相应的汇编语言实现该函数以获得最大效能
 */
long int GUI_refresh_pix(unsigned long int x, unsigned long int y)
{
	
	unsigned int new_color;
	unsigned char alpha = 0, red = 0, green = 0, blue = 0;
	struct layer *layer = layer_root;
	
	/**层叠像素*/
	do
	{
		/**指向下一个图层*/
		layer = layer->top;
		
		/**判断该图层是否被隐藏*/
		if (layer->visiable == true)
		{
			new_color = GUI_get_pix(layer, x - (*layer).x, y - (*layer).y);
			alpha = new_color >> 24;
			red = ((((new_color >> 16) & 0xff) * alpha) / 0xff) + ((red * (0xff - alpha)) / 0xff);
			green = ((((new_color >> 8) & 0xff) * alpha) / 0xff) + ((green * (0xff - alpha)) / 0xff);
			blue = (((new_color & 0xff) * alpha) / 0xff) + ((blue * (0xff - alpha)) / 0xff);
		}
	}while (layer != layer_root);

	/**写入显存中*/
	putpixel(x, y, (red << 16) + (green << 8) + (blue));
}

void GUI_refresh_block(unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	unsigned long save_x = x, save_y = y;
	
	for (; y < save_y + width; y ++)
	{
		for (x = save_x; x < save_x + length; x ++)
		{
			GUI_refresh_pix(x, y);
		}
	}
}

/*refresh all the layer_control*/
long int GUI_refresh(void)
{
	unsigned long x, y;
	for (x = 0; x < Video_Info.xres; x ++)
	{
		for (y = 0; y < Video_Info.yres; y ++)
		{
			GUI_refresh_pix(x, y);
		}
	}
}