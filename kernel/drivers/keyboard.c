/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer Keyboard support
 *Explorer/drivers/keryboard.c
 *version:Alpha
 *7/26/2014 5:25 PM
 */

#include <keyboard.h>
#include <arch.h>
#include <stdbool.h>

static unsigned char key_buffer[MAX_KEYBUFFER];
static unsigned long int untreated;
static unsigned char *write_point;
static unsigned char *read_point;

void init_keyboard(void)
{
	extern int_keyboard;
	/*initialize key flag*/
	L_Shift = false;
	R_Shift = false;
	CapsLock = false;
	/*initialize buffer*/
	untreated = 0;
	write_point = key_buffer;
	read_point = key_buffer;
	/*register8259*/
	register_PIC(1, &int_keyboard, "keyboard");
	//wait_KBC_sendready();
	//io_out8(port_keycmd, keycmd_write_mode);
	//wait_KBC_sendready();
	//io_out8(port_keydat, kbc_mode);
	return;
}

void wait_KBC_sendready(void)
{
	for(;;)
	{
		if ((io_in8(port_keysta) & keysta_send_notready) == 0)
		{
			break;
		}
	}
	return;
}

/**
 * This function made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * This function run when a interrupt of keyboard happen,
 * first,we get scan code of keyboard from port 0x60 and
 * save it.
 * Then, we try to move the code into the buffer of keyboard
 * if there is a space for the code(untreated != MAX_KEYBUFFER)
 * after that,the write_point need to point to next space.
 * 
 * in the last,we need to set key flag if the scan code is 
 * a function key.
 */

void int_keyboard_display(void)
{
	unsigned char key_val;
	key_val = io_in8(keycmd_write_mode);
	
	/**compare if it is a function key*/
	if (key_val == 0x2A)/*Left Shift is down*/
	{
		L_Shift = true;
	}else if (key_val == 0xAA)/*Left Shift is up*/{
		L_Shift = false;
	}
	
	/**
	 * make scan code add to buffer
	 */
	if (untreated != MAX_KEYBUFFER)
	{
		*write_point = key_val;
		if (write_point != (key_buffer + MAX_KEYBUFFER - 1))
		{
			write_point++;
		}else{
			write_point = key_buffer;
		}
		untreated++;
	}else{
		return;
	}
}

/**
 * This function made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * return keyboard scan code if there is a code in buffer.
 * wait if no scan code.
 */

unsigned char get_key(void)
{
	unsigned char retval;
	for (; untreated == 0;);
	retval = *read_point;
	if (read_point != (key_buffer + MAX_KEYBUFFER - 1))
	{
		read_point++;
	}else{
		read_point = key_buffer;
	}
	untreated--;
	return retval;
}


unsigned char key_to_ascii(unsigned char code)
{
	if ((code != 0x00) && (code < 0x80))
	{
		return key_table[code];
	}
	return 0;
}
