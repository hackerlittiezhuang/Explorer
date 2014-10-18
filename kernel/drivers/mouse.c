<<<<<<< HEAD
/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Rose<roselyu@163.com>
 *Explorer Êó±ê¿ØÖÆ
 *ExplorerOS/Kernel/drivers/mouse.c
 *version:Alpha
 *8/26/2014 10:30 AM
 */

#include "../include/mouse.h"
#include "../include/keyboard.h"
#include "../include/arch.h"
void enable_mouse(void)
{
	//Active
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return;
}
void init_mouse(void)
{
	extern	int_mouse;
	/*×¢²á8259*/

	register_PIC(12, &int_mouse, "Mouse");
  	//enable_mouse();
}

void int_mouse_display(void)
{
	//printk("int mouse:0x%X.\n", io_in8(KEYCMD_WRITE_MODE));
	printk("int mouse.\n");
=======
/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Rose<roselyu@163.com>
 *Explorer Êó±ê¿ØÖÆ
 *ExplorerOS/Kernel/drivers/mouse.c
 *version:Alpha
 *8/26/2014 10:30 AM
 */

#include "../include/mouse.h"
#include "../include/keyboard.h"
#include "../include/arch.h"
void enable_mouse(void)
{
	//Active
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return;
}
void init_mouse(void)
{
	extern	int_mouse;
	/*×¢²á8259*/

	register_PIC(12, &int_mouse, "Mouse");
  	//enable_mouse();
}

void int_mouse_display(void)
{
	//printk("int mouse:0x%X.\n", io_in8(KEYCMD_WRITE_MODE));
	printk("int mouse.\n");
>>>>>>> explorer/master
}