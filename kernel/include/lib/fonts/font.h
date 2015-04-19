/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com>
 * Explorer Font Header
 * Explorer/include/font.h
 * version:Alpha
 * 18/10/2014
 */


#ifndef FONT_H_
#define FONT_H_


struct font_info{
	char *name, *addr, *copr;
	unsigned long length, width;
};

#define SIZE_OF_FONT 10

void init_Font(void);
long register_font(struct font_info *font_info);
struct font_info *font(char *name);

#endif