/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Rose<roselyu@163.com>
 *Explorer Font Register
 *ExplorerOS/Kernel/lib/fonts/font.c
 *version:Alpha
 *18/10/2014
 */

#include <font.h>
#include <stdlib.h>

struct fontstr sysfont[FONTREGMAX];

void init_font(void)
{
	//Font Initialization
	unsigned char i;
	for (i = 0; i < FONTREGMAX; i++)
	{
		sysfont[i].font_name=NULL;
		sysfont[i].font_addr=NULL;
	}
	//Register all the fonts
		regfont("simsun",simsun);
		regfont("Standard Font",standard_font);
}

long regfont(unsigned char *name,unsigned char *addr)
{
	//Register a font
	unsigned long i;
	for (i = 0; i < FONTREGMAX; i++)
	{
		if (sysfont[i].font_name==NULL)
		{
			sysfont[i].font_name=name;
			sysfont[i].font_addr=addr;
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