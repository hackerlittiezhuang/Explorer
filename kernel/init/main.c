/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer main
 * ExplorerOS/Kernel/init/main.c
 * version:Alpha
 * 7/9/2014 8:30 AM
 */

#include <fat32.h>
#include <fat32.h>
#include <process.h>
#include <video.h>
#include <GUI.h>
#include <main.h>
#include <types.h>
#include <info.h>
#include <arch.h>
#include <keyboard.h>

/*main function of kernel*/
void main(void)
{
    unsigned char testnormal[] = "Exp";
	unsigned char detest[] = "lorer";
	/*显示版权、设备等信息*/
	/**
	 * display copyright and device etc information
	 */
	put_info();
	/*测试C++函数*/
	test_C_plus_plus();
		color(0xffff00);
	printk("this is a test.F%dck.%s",detest,testnormal);
	//init_hdd();
	//init_FAT32();
	/*加载文件*/
	//load_BACKDROP_PIC();
	//load_COPR();
	//
	//init_syscalls();
	//init_keyboard();
	//init_mouse();
	//init();
	//io_sti();
	//printk("tsc:%X",read_tsc());
	//printk("tsc:%X",read_tsc());
	
}

void put_info(void)
{
	color(0xffff00);
	printk("%s %s Author:[%s]\n", kernel_name, kernel_version, author);
	color(0xffffff);
	printk("%s\n", copyright);
	color(0xffff00);
	printk("Base: %s.\n", base);
	color(0xffffff);
	printk("%s\n\n", description);
	color(0xffffff);
}

/*load copyright from COPR.TXT*/
void load_COPR(void)
{
	u32 COPR_buffer;
	file_info COPR_info;
	COPR_buffer = (u32) kmalloc(COPR_info.size);
	file_open("COPR.TXT", COPR_buffer);
	printk(COPR_buffer);
	kfree(COPR_buffer, COPR_info.size);
}

/*load picture*/
void load_BACKDROP_PIC(void)
{
	/*initialization picture layer*/
	struct struct_layer *picture_layer;
	picture_layer = GUI_layer_alloc(&layer_root);
	(*picture_layer).visiable = true;
	GUI_set_position(picture_layer, 0, 0);
	
	/*notice:There are use immediately number to the function,
	 *They will be affect support in different VGA mode
	 */
	
	GUI_set_resolution(picture_layer, 800, 600);
	/*试加载BACKDROP.PIC*/
	file_open("BACKDROP.PIC", (u32) (*picture_layer).buf);//BACKDROP_PIC_point
	GUI_refresh(&layer_root);
	refresh_scr((unsigned char *) layer_root.buffer);
}
