<<<<<<< HEAD
/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer Graphical User Interface
 *Explorer 0.01/GUI/GUI.c
 *version:Alpha
 *8/16/2014 9:08 AM
 */

#include "../include/stdlib.h"
#include "../include/video.h"
#include "../include/GUI.h"

struct struct_layer *test_layer, *test2_layer;

/*initialization GUI*/
void init_GUI(void)
{
	/*notice:"buffer" and "test_buffer" are point to kmalloc function allocate memory
	 *which argument is immediately number,it will be affect different size support
	 */
	
	/*allocate a buffer for the layer_root*/
	unsigned int *buffer;
	buffer = (unsigned int *) kmalloc(800 * 600 * 4);
	/*initialization layer_root*/
	layer_root.width = 600;
	layer_root.length = 800;
	layer_root.top = 0;
	layer_root.buffer = buffer;
	layer_root.parent_layer = NULL;
	/*clean all the layer in the layer_root and point the control*/
	unsigned long i;
	for (i=0; i < MAX_LAYERS; i++)
	{
		layer_root.layer[i].flags = 0;
		layer_root.layer[i].parent_control = &layer_root;
	}	

	/*The code after this in the init_GUI function is to initialization Window*/
	handle window_1;
	window_1 = GUI_creat_Window(100, 100, "Hello,world");
}

/*allocate a layer from the layer_control*/
struct struct_layer *GUI_layer_alloc(struct layer_control *layer_control)
{
	unsigned long i = (*layer_control).top;
	if (i != MAX_LAYERS)
	{
		(*layer_control).layer[i].flags = 1;
		(*layer_control).layer[i].visiable = false;
		(*layer_control).top++;
		return &(*layer_control).layer[i];
	}else{
		return NULL;
	}
}

/*change the height of the layer*/
long int GUI_set_height(struct struct_layer *layer, unsigned long int height)
{
	
}

/*change the position of the layer*/
long int GUI_set_position(struct struct_layer *layer, unsigned long x, unsigned long y)
{
	(*layer).offset_x = x;
	(*layer).offset_y = y;
	GUI_refresh((*layer).parent_control);
}

/*change the resolution of the layer*/
long int GUI_set_resolution(struct struct_layer *layer, unsigned long length, unsigned long width)
{
	/*If it had buffer,we must free first*/
	if ((*layer).buf != NULL)
	{
		oldkfree((*layer).buf, (*layer).width * (*layer).length * 4);
	}
	(*layer).buf = (unsigned int *) kmalloc(length * width * 4);
	(*layer).length = length;
	(*layer).width = width;
	return 0;
}

long int GUI_refresh_pix(struct layer_control *layer_control, long int x, long int y)
{
	unsigned long h;
	unsigned int new_color, color = 0x0;
	unsigned char alpha, red = 0, green = 0, blue = 0;
	struct struct_layer *layer;
	for (h = 0; h < (*layer_control).top; h++)
	{
		layer = &(*layer_control).layer[h];
		if ((*layer).visiable == true)
		{
			new_color = GUI_get_pix(layer, x - (*layer).offset_x, y - (*layer).offset_y);
			alpha = new_color >> 24;
			red = ((((new_color >> 16) & 0xff) * alpha) / 0xff) + ((red * (0xff - alpha)) / 0xff);
			green = ((((new_color >> 8) & 0xff) * alpha) / 0xff) + ((green * (0xff - alpha)) / 0xff);
			blue = (((new_color & 0xff) * alpha) / 0xff) + ((blue * (0xff - alpha)) / 0xff);
			//color = ((alpha * new_color) >> 8) ;//+ (((0xff - alpha) * color) >> 8);	
		}
	}
	color = (red << 16) + (green << 8) + (blue);
	(*layer_control).buffer[(y * (*layer_control).length) + x] = color;
	put_pix_24(x, y, color);
}

/*refresh all the layer_control*/
long int GUI_refresh(struct layer_control *layer_control)
{
	unsigned long int h, x, y, buffer_point, buf_point;
	unsigned char alpha, *buffer, *buf;
	buffer = (unsigned char *) (*layer_control).buffer;
	/*clean the buffer*/
	unsigned long i;
	for (i = 0; i < ((*layer_control).length * (*layer_control).width); i++)
	{
		(*layer_control).buffer[i] = 0x0;
	}
	struct struct_layer *layer;
	for (h = 0; h < (*layer_control).top; h++)
	{
		layer = &(*layer_control).layer[h];
		buf = (unsigned char *) (*layer).buf;
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
							buf_point = (y * (*layer).length + x) * 4;
							buffer_point = ((((*layer).offset_y + y) * (*layer_control).length) + (*layer).offset_x + x) * 4;
							alpha = buf[buf_point + 3];
							buffer[buffer_point] = ((buf[buf_point] * alpha) / 0xff) + ((buffer[buffer_point] * (0xff - (alpha))) / 0xff);
							buffer[buffer_point + 1] = ((buf[buf_point + 1] * alpha) / 0xff) + ((buffer[buffer_point + 1] * (0xff - (alpha))) / 0xff);
							buffer[buffer_point + 2] = ((buf[buf_point + 2] * alpha) / 0xff) + ((buffer[buffer_point + 2] * (0xff - (alpha))) / 0xff);
							buffer[buffer_point + 3] = 0xff;
						}
					}
				}
			}
		}
	}
	refresh_scr((unsigned char *) (*layer_control).buffer);
}

/*draw a point in the layer*/
long int GUI_put_pix(struct struct_layer *layer, unsigned int color, unsigned long x, unsigned long y)
{
	/*There is a judgement about if the pix in the layer*/
	/*If there isn't a judgement,the pix may be overflow the buffer*/
	if ((x >= (*layer).length) | (y >= (*layer).width)) return -1;
	(*layer).buf[(y * (*layer).length) + x] = color;
	/*refresh the pix*/
	GUI_refresh_pix((*layer).parent_control, (*layer).offset_x + x, (*layer).offset_y + y);
	return 0;
}

/*draw a square in the layer*/
long int GUI_put_square(struct struct_layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	/*Two pointers to determine coordinates*/
	unsigned long int x_point, y_point;
	/*Don't worried about the square overflow the buffer
	 *because every pix check in the GUI_put_pix function
	 */
	for (y_point = 0; y_point < width; y_point++)
	{
		for (x_point = 0; x_point < length; x_point++)
		{
			GUI_put_pix(layer, color, (x + x_point), (y + y_point));
		}
	}
}

unsigned int GUI_get_pix(struct struct_layer *layer, unsigned long x, unsigned long y)
{
	/*There is a judgement about if the pix in the layer*/
	/*If there isn't a judgement,the pix may be overflow the buffer*/
	if ((x >= (*layer).length) | (y >= (*layer).width)) return 0;
	return (*layer).buf[(y * (*layer).length) + x];
}

handle GUI_creat_Window(unsigned long x, unsigned long y, unsigned char *title)
{
	//struct struct_window *struct_window
=======
/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer Graphical User Interface
 *Explorer 0.01/GUI/GUI.c
 *version:Alpha
 *8/16/2014 9:08 AM
 */

#include "../include/stdlib.h"
#include "../include/video.h"
#include "../include/GUI.h"

struct struct_layer *test_layer, *test2_layer;

/*initialization GUI*/
void init_GUI(void)
{
	/*notice:"buffer" and "test_buffer" are point to kmalloc function allocate memory
	 *which argument is immediately number,it will be affect different size support
	 */
	
	/*allocate a buffer for the layer_root*/
	unsigned int *buffer;
	buffer = (unsigned int *) kmalloc(800 * 600 * 4);
	/*initialization layer_root*/
	layer_root.width = 600;
	layer_root.length = 800;
	layer_root.top = 0;
	layer_root.buffer = buffer;
	layer_root.parent_layer = NULL;
	/*clean all the layer in the layer_root and point the control*/
	unsigned long i;
	for (i=0; i < MAX_LAYERS; i++)
	{
		layer_root.layer[i].flags = 0;
		layer_root.layer[i].parent_control = &layer_root;
	}	

	/*The code after this in the init_GUI function is to initialization Window*/
	handle window_1;
	window_1 = GUI_creat_Window(100, 100, "Hello,world");
}

/*allocate a layer from the layer_control*/
struct struct_layer *GUI_layer_alloc(struct layer_control *layer_control)
{
	unsigned long i = (*layer_control).top;
	if (i != MAX_LAYERS)
	{
		(*layer_control).layer[i].flags = 1;
		(*layer_control).layer[i].visiable = false;
		(*layer_control).top++;
		return &(*layer_control).layer[i];
	}else{
		return NULL;
	}
}

/*change the height of the layer*/
long int GUI_set_height(struct struct_layer *layer, unsigned long int height)
{
	
}

/*change the position of the layer*/
long int GUI_set_position(struct struct_layer *layer, unsigned long x, unsigned long y)
{
	(*layer).offset_x = x;
	(*layer).offset_y = y;
	GUI_refresh((*layer).parent_control);
}

/*change the resolution of the layer*/
long int GUI_set_resolution(struct struct_layer *layer, unsigned long length, unsigned long width)
{
	/*If it had buffer,we must free first*/
	if ((*layer).buf != NULL)
	{
		oldkfree((*layer).buf, (*layer).width * (*layer).length * 4);
	}
	(*layer).buf = (unsigned int *) kmalloc(length * width * 4);
	(*layer).length = length;
	(*layer).width = width;
	return 0;
}

long int GUI_refresh_pix(struct layer_control *layer_control, long int x, long int y)
{
	unsigned long h;
	unsigned int new_color, color = 0x0;
	unsigned char alpha, red = 0, green = 0, blue = 0;
	struct struct_layer *layer;
	for (h = 0; h < (*layer_control).top; h++)
	{
		layer = &(*layer_control).layer[h];
		if ((*layer).visiable == true)
		{
			new_color = GUI_get_pix(layer, x - (*layer).offset_x, y - (*layer).offset_y);
			alpha = new_color >> 24;
			red = ((((new_color >> 16) & 0xff) * alpha) / 0xff) + ((red * (0xff - alpha)) / 0xff);
			green = ((((new_color >> 8) & 0xff) * alpha) / 0xff) + ((green * (0xff - alpha)) / 0xff);
			blue = (((new_color & 0xff) * alpha) / 0xff) + ((blue * (0xff - alpha)) / 0xff);
			//color = ((alpha * new_color) >> 8) ;//+ (((0xff - alpha) * color) >> 8);	
		}
	}
	color = (red << 16) + (green << 8) + (blue);
	(*layer_control).buffer[(y * (*layer_control).length) + x] = color;
	put_pix_24(x, y, color);
}

/*refresh all the layer_control*/
long int GUI_refresh(struct layer_control *layer_control)
{
	unsigned long int h, x, y, buffer_point, buf_point;
	unsigned char alpha, *buffer, *buf;
	buffer = (unsigned char *) (*layer_control).buffer;
	/*clean the buffer*/
	unsigned long i;
	for (i = 0; i < ((*layer_control).length * (*layer_control).width); i++)
	{
		(*layer_control).buffer[i] = 0x0;
	}
	struct struct_layer *layer;
	for (h = 0; h < (*layer_control).top; h++)
	{
		layer = &(*layer_control).layer[h];
		buf = (unsigned char *) (*layer).buf;
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
							buf_point = (y * (*layer).length + x) * 4;
							buffer_point = ((((*layer).offset_y + y) * (*layer_control).length) + (*layer).offset_x + x) * 4;
							alpha = buf[buf_point + 3];
							buffer[buffer_point] = ((buf[buf_point] * alpha) / 0xff) + ((buffer[buffer_point] * (0xff - (alpha))) / 0xff);
							buffer[buffer_point + 1] = ((buf[buf_point + 1] * alpha) / 0xff) + ((buffer[buffer_point + 1] * (0xff - (alpha))) / 0xff);
							buffer[buffer_point + 2] = ((buf[buf_point + 2] * alpha) / 0xff) + ((buffer[buffer_point + 2] * (0xff - (alpha))) / 0xff);
							buffer[buffer_point + 3] = 0xff;
						}
					}
				}
			}
		}
	}
	refresh_scr((unsigned char *) (*layer_control).buffer);
}

/*draw a point in the layer*/
long int GUI_put_pix(struct struct_layer *layer, unsigned int color, unsigned long x, unsigned long y)
{
	/*There is a judgement about if the pix in the layer*/
	/*If there isn't a judgement,the pix may be overflow the buffer*/
	if ((x >= (*layer).length) | (y >= (*layer).width)) return -1;
	(*layer).buf[(y * (*layer).length) + x] = color;
	/*refresh the pix*/
	GUI_refresh_pix((*layer).parent_control, (*layer).offset_x + x, (*layer).offset_y + y);
	return 0;
}

/*draw a square in the layer*/
long int GUI_put_square(struct struct_layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	/*Two pointers to determine coordinates*/
	unsigned long int x_point, y_point;
	/*Don't worried about the square overflow the buffer
	 *because every pix check in the GUI_put_pix function
	 */
	for (y_point = 0; y_point < width; y_point++)
	{
		for (x_point = 0; x_point < length; x_point++)
		{
			GUI_put_pix(layer, color, (x + x_point), (y + y_point));
		}
	}
}

unsigned int GUI_get_pix(struct struct_layer *layer, unsigned long x, unsigned long y)
{
	/*There is a judgement about if the pix in the layer*/
	/*If there isn't a judgement,the pix may be overflow the buffer*/
	if ((x >= (*layer).length) | (y >= (*layer).width)) return 0;
	return (*layer).buf[(y * (*layer).length) + x];
}

handle GUI_creat_Window(unsigned long x, unsigned long y, unsigned char *title)
{
	//struct struct_window *struct_window
>>>>>>> explorer/master
}