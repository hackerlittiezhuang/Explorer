/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Rose<roselyu@163.com>
 *Explorer Font Register
 *ExplorerOS/Kernel/lib/fonts/font.c
 *version:Alpha
 *18/10/2014
 */

#include <lib/fonts/font.h>
#include <lib/string.h>
#include <stdlib.h>

struct fontreg sysfont[FONTREGMAX];

void init_font(void)
{
	//Font Initialization
	unsigned char i;
	for (i = 0; i < FONTREGMAX; i++)
	{
		sysfont[i].font_name=NULL;
		sysfont[i].font_addr=NULL;
		sysfont[i].intro=NULL;
		sysfont[i].height=16;
		sysfont[i].width=8;
	}
	//Reserved Font
	sysfont[0].font_name="null font";
	sysfont[0].font_addr=NULL;
	sysfont[0].height=0;
	sysfont[0].width=0;
	sysfont[0].intro=NULL;
	//Register all the fonts
		Register_simsun();
		Register_standard_font();
		Register_linux_sun();
}

long regfont(unsigned char *font_name,unsigned char *font_addr,unsigned int width,unsigned int height,unsigned char *intro)
{
	//Register a font
	unsigned long i;
	for (i = 1; i < FONTREGMAX; i++)
	{
		if (sysfont[i].font_name==NULL)
		{
			sysfont[i].font_name=font_name;
			sysfont[i].font_addr=font_addr;
			sysfont[i].height=height;
			sysfont[i].width=width;
			sysfont[i].intro=intro;
			return 0;
		}
	}
	return -1;
}

unsigned char *get_font_addr(unsigned char *name)
{
	//Get font information
	unsigned long i;
	for (i = 0; i < FONTREGMAX; i++)
	{
		if (strncmp(sysfont[i].font_name,name, strlen(sysfont[i].font_name))==0)
		{
			return sysfont[i].font_addr;
		}
	}
	return NULL;
}

struct fontreg *get_font_info(unsigned char *name)
{
	unsigned long i;
	for (i = 0; i < FONTREGMAX; i++)
	{
		if (strncmp(sysfont[i].font_name,name, strlen(sysfont[i].font_name))==0)
		{
			return &sysfont[i];
		}
	}
	return NULL;
}