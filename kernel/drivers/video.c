/** * Copyright 2013-2015 by Explorer Developers. * made by Lab Explorer Developers<1@GhostBirdOS.org> * Explorer Standard Video Support * Explorer/drivers/video.c * version:Alpha * 7/10/2014 8:30 AM */#include <types.h>#include <lib/graphics.h>#include "video.h"/**Initialize for vesa*/void init_VESA(void){	//page 22	Video_Info.xres = 800;	Video_Info.yres = 600;	Video_Info.bit_per_pixel = 24;	Video_Info.vram_length = (((Video_Info.xres * Video_Info.yres) * (Video_Info.bit_per_pixel / 8)) & 0xfffff000) + 0x1000;	Video_Info.vram = (char *)0xe0000000;//(char *) kmalloc(vram_length);	//Video_Info.vram = (u8 *)vmalloc(Video_Info.vram_length);	//kmap(Video_Info.vram, 0xe0000000, Video_Info.vram_length);}/*refresh all the video RAM function*/void refresh_scr(unsigned char *buffer){	unsigned char *vram = Video_Info.vram;	unsigned long vram_length = Video_Info.vram_length;	unsigned int buf_point = 0, vram_point = 0;	while (vram_point < vram_length)	{		Video_Info.vram[vram_point] = (buffer[buf_point+3] * buffer[buf_point]) / 0xff;/*蓝通道*/		Video_Info.vram[vram_point+1] = (buffer[buf_point+3] * buffer[buf_point+1]) / 0xff;/*蓝通道*/		Video_Info.vram[vram_point+2] = (buffer[buf_point+3] * buffer[buf_point+2]) / 0xff;/*蓝通道*/		buf_point += 4;/*模拟32位图形模式*/		vram_point += (Video_Info.bit_per_pixel / 8);	}}void refresh_pix_32(unsigned long x, unsigned int long y, unsigned int color){	int long i;	unsigned char alpha;	unsigned char *vram = Video_Info.vram;	if (x < Video_Info.xres & y < Video_Info.yres)	{		alpha = color >> 24;		i = ((y * Video_Info.xres) + x)*3;		Video_Info.vram[i] = (alpha * color) / 0xff;		Video_Info.vram[i+1] = ((alpha * color) >> 8) / 0xff;		Video_Info.vram[i+2] = ((alpha * color) >> 16) / 0xff;	}}unsigned int get_pix_24(unsigned int x, unsigned int y){	unsigned char *vram = Video_Info.vram;	unsigned int i;	if (x < Video_Info.xres & y < Video_Info.yres)	{		i = ((y * Video_Info.xres) + x)*3;		return (Video_Info.vram[i] + (Video_Info.vram[i+1] << 8) + (Video_Info.vram[i+2] << 16));	}}