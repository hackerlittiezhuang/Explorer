/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com>
 * Explorer Font Register
 * Explorer/lib/fonts/font.c
 * version:Alpha
 * 18/10/2014
 */

#include <lib/fonts/font.h>
#include <lib/string.h>
#include <stdlib.h>

/**字库列表*/
struct font_info font_lib[SIZE_OF_FONT];

void init_Font(void)
{
	/**empty the list*/
	unsigned long n;
	for (n = 0; n < SIZE_OF_FONT; n++)
	{
		font_lib[n].name = NULL;
	}
	
	/**register font*/
	Register_standard_font();
	Register_simsun();
}

/**注册字库*/
long register_font(struct font_info *font_info)
{
	unsigned long n;
	for (n = 0; n < SIZE_OF_FONT; n++)
	{
		if (font_lib[n].name == NULL) goto empty_font;
	}
	return -1;
	
empty_font:
	font_lib[n] = *font_info;
	return 0;
}

struct font_info *font(char *name)
{
	unsigned long n;
	
	for (n = 0; n < SIZE_OF_FONT; n++)
	{
		if (strncmp(font_lib[n].name, name, strlen(font_lib[n].name)) == 0)
		{
			return &font_lib[n];
		}
	}
	
	return NULL;
}
