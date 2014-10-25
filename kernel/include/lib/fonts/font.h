/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Rose<roselyu@163.com>
 *Explorer Font Header
 *ExplorerOS/include/font.h
 *version:Alpha
 *18/10/2014
 */
#ifndef FONT_H_
#define FONT_H_
// Font Structure
// *font_name : name of the font
// *font_addr : address of the font
//

#define FONTREGMAX 10
//Max Number of sysfont array members
struct fontreg{
unsigned char *font_name;
unsigned char *font_addr;
unsigned int width;
unsigned int height;
unsigned char *intro;
};
//Font is a structure of font register.
//Sysfont is the default font register.
void init_font(void);
long regfont(unsigned char *font_name,unsigned char *font_addr,unsigned int width,unsigned int height,unsigned char *intro);
//get_font_addr will be reserved till when shell supports customize font width and height.
unsigned char *get_font_addr(unsigned char *name);
struct fontreg *get_font_info(unsigned char *name);
/**Font Register Function*/
void Register_simsun(void);
void Register_standard_font(void);

#endif