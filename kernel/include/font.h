/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Rose<roselyu@163.com>
 *Explorer Font Register
 *ExplorerOS/Kernel/lib/font.c
 *version:Alpha
 *18/10/2014
 */
#pragma
// Font Structure
// *font_name : name of the font
// *font_addr : address of the font
//

struct fontstr{
unsigned char *font_name;
unsigned char *font_addr;
};

void init_font(void);
long regfont(char *name,char *addr);