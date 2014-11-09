/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer main
 * ExplorerOS/Kernel/init/main.c
 * version:Alpha
 * 7/9/2014 8:30 AM
 */

#include <fat32.h>
#include <video.h>
#include <GUI.h>
#include <main.h>
#include <types.h>
#include <info.h>
#include <arch.h>
#include <keyboard.h>
#include <kmalloc.h>

/*main function of kernel*/
void main(void)
{
	/**display copyright and device etc information*/
	put_info();
	/*测试C++函数*/
	//test_C_plus_plus();
	init_hdd();
	init_FAT32();
	/*加载文件*/
	//load_COPR();
	//load_BACKDROP_PIC();
	load_ELF();
	printk("\nCREDIT\n %s\n", author);
	//
	//init_syscalls();
}

void load_ELF(void)
{
	u32 ELF_buffer;
	file_info ELF_info;
	void (*sb)(void);
	
	//Code Begin
	sb=kmalloc(ELF_info.size); //Get the address
	//ELF_buffer = (u32) *sb; //Convert to u32
	file_open("EXPLORER.ELF", sb); //Load ELF to memory
	sb = sb + 0x32;
	(*sb)();
	//oldkfree(ELF_buffer, ELF_info.size);
	printk("sb");
	outtextxy(200, 500, 0xffffff, "sbsbsbAHs0/18/2014");
}

void put_info(void)
{
	int p;
	p=printk("                                            ");
	printk("\n%s %s - %s.\n\n", kernel_name, kernel_version,base);
	printk("(C)Copyright %s. All rights reserved.\n", copyright);
	printk("Description: %s\n", description);
	printk("\nCREDIT\n %s\n", author);
	output_CPU_info();
	cmos_info();
}

/*load copyright from COPR.TXT*/
void load_COPR(void)
{
	u32 COPR_buffer;
	file_info COPR_info;
	COPR_buffer = (u32) kmalloc(COPR_info.size);
	file_open("EXPLORER.ELF", COPR_buffer);
	printk(COPR_buffer);
	oldkfree(COPR_buffer, COPR_info.size);
}

/*load picture*/
void load_BACKDROP_PIC(void)
{
	char *pic_buffer;
	pic_buffer = oldkmalloc(800*600*3);
	/*试加载BACKDROP.PIC*/
	file_open("BACKDROP.PIC", (u32)vram);//BACKDROP_PIC_point
	//refresh_scr((unsigned char *) pic_buffer);
}

void cmos_info(void)
{
	char i;
	char *pointer;
	char byte;
	pointer = (char *) &cmos;
	for (i = 0; i < 0x34; i++)
	{
		io_out8(0x70, i);
		byte = io_in8(0x71);
		*pointer++ = byte;
	}
	printk(" CMOS infomation:\n");
	//date
	printk(" >> Date: %X/%X/%X", cmos.current_month,
		cmos.current_day, cmos.century);
	if(cmos.current_year<10)
	 printk("0%X.\n",cmos.current_year);
	else
	 printk("%X.\n",cmos.current_year);
	
	//time 00:00:00
	printk(" >> Time: ");
	if(cmos.current_hour<10)
	 printk("0%X:",cmos.current_hour);
	else
	 printk("%X:",cmos.current_hour);
	 
	if(cmos.current_minute<10)
	 printk("0%X:",cmos.current_minute);
	else
	 printk("%X:",cmos.current_minute);
	
	if(cmos.current_second<10)
	 printk("0%X",cmos.current_second);
	else
	 printk("%X",cmos.current_second);
	printk(".\n");
}

void second(void)
{
	//Real-time Second Refresh
	char i;
	char *pointer;
	char byte;
	pointer = (char *) &cmos;
	for (i = 0; i < 0x34; i++)
	{
		io_out8(0x70, i);
		byte = io_in8(0x71);
		*pointer++ = byte;
	}
	printk(" >> Time: ");
	if(cmos.current_hour<10)
	 printk("0%X:",cmos.current_hour);
	else
	 printk("%X:",cmos.current_hour);
	 
	if(cmos.current_minute<10)
	 printk("0%X:",cmos.current_minute);
	else
	 printk("%X:",cmos.current_minute);
	
	if(cmos.current_second<10)
	 printk("0%X",cmos.current_second);
	else
	 printk("%X",cmos.current_second);
	printk(".\n");
	
}
