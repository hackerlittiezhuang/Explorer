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
#define FONTDATAMAX 4096
struct fontstr{
unsigned char *font_name;
unsigned char *font_addr;
};

void init_font(void);
long regfont(unsigned char *name,unsigned char *addr);
unsigned char *get_font_addr(unsigned char *name);
//Other Fonts here.
unsigned char standard_font[FONTDATAMAX];
unsigned char simsun[FONTDATAMAX];