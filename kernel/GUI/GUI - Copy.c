/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by 迷彩红星<1@GhostBirdOS.org>
 *Explorer 主函数
 *ExplorerOS/Kernel/GUI/GUI.c
 *version:Alpha
 *8/16/2014 9:08 AM
 */

#include "../include/stdlib.h"
#include "../include/video.h"
#include "../include/GUI.h"

struct layer_control layer_root;

/*初始化GUI*/
void init_GUI(void)
{
	/*初始化根图层的相关信息*/
	layer_root.width = 600;
	layer_root.length = 800;
	layer_root.top = 0;
	layer_root.buffer = (unsigned int *) kmalloc(vram_len);
	/*将所有layer_root中的图层标记为未使用*/
	unsigned long i;
	for (i=0; i < MAX_LAYERS; i++)
	{
		layer_root.layer[i].flags = 0;
	}
	/*申请测试图层*/
	struct struct_layer *test_layer, *test_layer2;
	test_layer = GUI_layer_alloc(&layer_root);
	//test_layer2 = GUI_layer_alloc(&layer_root);
	GUI_set_position(test_layer, 0, 0);
	GUI_set_resolution(test_layer, 100, 100);
	//GUI_set_resolution(test_layer2, 800, 600);
	/*图层绘画*/
	//GUI_put_pix(test_layer, 0xffffffff, 10, 10);
	//GUI_put_pix(test_layer, 0xffffffff, 0, 1);
	//GUI_put_pix(test_layer, 0xffffffff, 1, 0);
	//GUI_put_pix(test_layer, 0xffffffff, 1, 1);
	//GUI_put_square(test_layer, 0xffffffff, 0, 0, 100, 100);
	//GUI_put_square(test_layer2, 0xffffff00, 100, 100, 100, 100);
	//(*test_layer).visiable = true;
	//(*test_layer2).visiable = true;
	
	layer_root.buffer[0] = 0xffffffff;
	layer_root.buffer[2] = 0xffffffff;
	layer_root.buffer[3] = 0xffffffff;
	
	/*刷新*/
	GUI_refresh(&layer_root);
	refresh_scr((unsigned char *) layer_root.buffer);
}

struct struct_layer *GUI_layer_alloc(struct layer_control *layer_control)
{
	unsigned long i;
	for (i=0; i < MAX_LAYERS; i++)
	{
		if ((*layer_control).layer[i].flags == 0)
		{
			(*layer_control).layer[i].flags = 1;
			//(*layer_control).layer[i].visiable = false;
			(*layer_control).top++;
			return &(*layer_control).layer[i];
		}
	}
	return NULL;
}

long int GUI_set_position(struct struct_layer *layer, unsigned long x, unsigned long y)
{
	(*layer).offset_x = x;
	(*layer).offset_y = y;
}

long int GUI_set_resolution(struct struct_layer *layer, unsigned long length, unsigned long width)
{
	/*如果已经有缓冲区，就释放*/
	if ((*layer).buf != NULL)
	{
		kfree((*layer).buf, (*layer).width * (*layer).length * (bpp / 8));
	}
	(*layer).buf = (unsigned int *) kmalloc(length * width * (bpp / 8));
	(*layer).length = length;
	(*layer).width = width;
	return 0;
}

long int GUI_refresh(struct layer_control *layer_control)
{
	unsigned long h, x, y;
	struct struct_layer *layer;
	for (h = 0; h <= (*layer_control).top; h ++)
	{
		layer = &((*layer_control).layer[h]);
		if ((*layer).visiable == true)
		{
			for (y = 0; y < (*layer).width; y++)
			{
				if ((y + (*layer).offset_y) < (*layer_control).width)
				{
					for (x = 0; x < (*layer).length; x++)
					{
						if ((x + (*layer).offset_x) < (*layer_control).length)
						{
							(*layer_control).buffer[(((*layer).offset_y + y) * (*layer_control).length) + (*layer).offset_x + x] = (*layer).buf[y * (*layer).length + x];
							//(*layer_control).buffer[0] = 0xffffffff;//(*layer).buf[y * (*layer).length + x];
						}
					}
				}
			}
		}
	}
}

long int GUI_put_square(struct struct_layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	unsigned long m,n;
	for (m = 0; m < width; m++)
	{
		for (n = 0; n < length; n++)
		{
			GUI_put_pix(layer, color, (n + x), (m + y));
		}
	}
}

long int GUI_put_pix(struct struct_layer *layer, unsigned int color, unsigned long x, unsigned long y)
{
	if ((x < (*layer).length) && (y < (*layer).width))
	{
		(*layer).buf[(y * (*layer).length) + x] = color;
	}
	printk("GUI_put_pix:color=%X,x=%X,y=%X,Offset:%X.\n", color, x, y, ((y * (*layer).length) + x));
}