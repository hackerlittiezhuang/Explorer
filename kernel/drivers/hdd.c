<<<<<<< HEAD
/*
 *Copyright 2013-2014 by 2013-2014 by Explorer OS Developer. All rights reserved.

 *Explorer 0.01 磁盘管理
 *File name:Explorer\Kernel\drivers\hdd.c
 *2014.7.18 5:48 PM
 */
 
#include <hdd.h>
#include <types.h>
#include <stdlib.h>
#include <kmalloc.h>


unsigned int LBA_start;

void init_hdd(void)
{
	extern int_HDC;
	/*set interrupt*/
	register_PIC(0xe, &int_HDC, "Hard Disk Control");
	u8 *point;
	point = (u8 *) oldkmalloc(512);
	/*if (point == NULL)
	{
		printk("HDD:memory allocate error!");
		io_hlt();
	}*/
	read_disk(0, (unsigned short int*) point, 1);
	kmemcpy((point + 0x1be + 8), &LBA_start, 2);
	oldkfree(point, 512);
}

void read_disk(u32 LBA, u16 *buffer, u32 number)
{
	u32 offset, i;
	io_out16(0x1f2,number);/*数量*/
	io_out8(0x1f3,(LBA & 0xff));/*LBA地址7~0*/
	io_out8(0x1f4,((LBA >> 8) & 0xff));/*LBA地址15~8*/
	io_out8(0x1f5,((LBA >> 16) & 0xff));/*LBA地址23~16*/
	io_out8(0x1f6,(((LBA >> 24) & 0xff) + 0xe0));/*LBA地址27~24 + LBA模式，主硬盘*/
	io_out8(0x1f7,0x20);/*读扇区*/
	for (i = 0; i != number; i ++)
	{
		hdd_wait();
		for (offset = 0; offset < 256; offset ++)
		{
			buffer[(i * 256) + offset] = io_in16(0x1f0);
		}
	}
	return;
}

void hdd_wait(void)
{
	for (; (io_in8(0x1f7) & 0x88) != 0x08;);/*循环等待*/
}

void int_HDC_display(void)
{
	printk("int HDC.\n");
=======
/*
 *Copyright 2013-2014 by 2013-2014 by Explorer OS Developer. All rights reserved.

 *Explorer 0.01 磁盘管理
 *File name:Explorer\Kernel\drivers\hdd.c
 *2014.7.18 5:48 PM
 */
 
#include <hdd.h>
#include <types.h>
#include <stdlib.h>
#include <kmalloc.h>


unsigned int LBA_start;

void init_hdd(void)
{
	extern int_HDC;
	/*set interrupt*/
	register_PIC(0xe, &int_HDC, "Hard Disk Control");
	u8 *point;
	point = (u8 *) oldkmalloc(512);
	/*if (point == NULL)
	{
		printk("HDD:memory allocate error!");
		io_hlt();
	}*/
	read_disk(0, (unsigned short int*) point, 1);
	kmemcpy((point + 0x1be + 8), &LBA_start, 2);
	oldkfree(point, 512);
}

void read_disk(u32 LBA, u16 *buffer, u32 number)
{
	u32 offset, i;
	io_out16(0x1f2,number);/*数量*/
	io_out8(0x1f3,(LBA & 0xff));/*LBA地址7~0*/
	io_out8(0x1f4,((LBA >> 8) & 0xff));/*LBA地址15~8*/
	io_out8(0x1f5,((LBA >> 16) & 0xff));/*LBA地址23~16*/
	io_out8(0x1f6,(((LBA >> 24) & 0xff) + 0xe0));/*LBA地址27~24 + LBA模式，主硬盘*/
	io_out8(0x1f7,0x20);/*读扇区*/
	for (i = 0; i != number; i ++)
	{
		hdd_wait();
		for (offset = 0; offset < 256; offset ++)
		{
			buffer[(i * 256) + offset] = io_in16(0x1f0);
		}
	}
	return;
}

void hdd_wait(void)
{
	for (; (io_in8(0x1f7) & 0x88) != 0x08;);/*循环等待*/
}

void int_HDC_display(void)
{
	printk("int HDC.\n");
>>>>>>> explorer/master
}