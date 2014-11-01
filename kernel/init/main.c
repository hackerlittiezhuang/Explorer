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
	//init_hdd();
	//init_FAT32();
	/*加载文件*/
	//load_BACKDROP_PIC();
	//load_COPR();
	//
	//init_syscalls();
}

void put_info(void)
{
	printk("%s %s - Based on %s.\n\n", kernel_name, kernel_version,base);
	printk("(C)Copyright %s. All rights reserved.\n", copyright);
	printk("Description: %s\n", description);
	printk("\nCREDIT\n %s\n", author);
}

/*load copyright from COPR.TXT*/
void load_COPR(void)
{
	u32 COPR_buffer;
	file_info COPR_info;
	COPR_buffer = (u32) kmalloc(COPR_info.size);
	file_open("COPR.TXT", COPR_buffer);
	printk(COPR_buffer);
	oldkfree(COPR_buffer, COPR_info.size);
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
