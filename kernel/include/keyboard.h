/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *modified by Rose<roselyu@163.com>
 *Explorer Keyboard support head
 *ExplorerOS/Kernel/include/keyboard.h
 *version:Alpha
 *7/18/2014 5:48 PM
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdbool.h>

#define MAX_KEYBUFFER			128

#define port_keydat				0x0060
#define port_keysta				0x0064
#define port_keycmd				0x0064
#define keysta_send_notready	0x02
#define keycmd_write_mode		0x60
#define kbc_mode				0x47

void init_keyboard(void);
void wait_KBC_sendready(void);
void int_keyboard_display(void);
unsigned char get_key(void);
unsigned char key_to_ascii(unsigned char code);

bool L_Shift;
bool R_Shift;
bool CapsLock;

static unsigned char key_table[0x7f] = {
	0x00,
	0x1B,
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
	'-',
	'=',
	0x08,
	0x09,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'[',
	']',
	0x0D,
	0x00,
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	';',
	0x60,
	0x00,
	0x00,
	0x5c,
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',
	',',
	'.',
	'/',
	0x00,
	'*',
	0x00,
	' ',
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0X00,
	0X00,
	'7',
	'8',
	'9',
	'-',
	'4',
	'5',
	'6',
	'+',
	'1',
	'2',
	'3',
	'0',
	'.',
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	'_',
	0X00,
	0X00,
	0X00,
	0X00,
	0X00,
	0X00,
	0X00,
	0X00,
	0x5C,
	0X00,
	0x00,
	0X00
	
};

#endif