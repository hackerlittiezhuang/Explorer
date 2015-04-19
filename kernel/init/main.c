/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Function main
 * Explorer/init/main.c
 * version:Alpha
 * 7/9/2014 8:30 AM
 */

/**
 * Explorer操作系统内核初始化主函数
 * 进行相关的初始化工作
 */

#include <lib/mem.h>
#include <stdarg.h>
#include <stdlib.h>
#include <../fs/fat32/fat32.h>
#include <GUI.h>
#include <main.h>
#include <types.h>
#include <info.h>
#include <arch.h>
#include <keyboard.h>
#include <memory.h>

/**任务测试函数*/
void task_test(void *arg)
{
	unsigned long n;
	for (;;){
		//for (n = 0; n < 0xffffff; n++);
		printk("%d.", get_id());
	}
}

/**内核主函数*/
void main(struct boot_info *boot_info)		/**启动信息结构体在Explorer/include/main.h中定义*/
{
	/**判断是否成功获取启动信息*/
	if (boot_info->flag[0] != 'E' |
		boot_info->flag[1] != 'B' |
		boot_info->flag[2] != 'I' )
	{
		reset();		/**系统重置*/
	}
	
	/**判断启动信息的长度是否符合要求*/
	if (boot_info->size != sizeof(struct boot_info))
	{
		reset();		/**系统重置*/
	}
	
	/**初始化内存管理单元*/
	init_MMU(boot_info);
	
	init_Interrupt();
	init_task();
	init_CPU();
	init_PIC();
	init_PIT();
	Inti_Graph();
	
	/**初始化字库*/
	init_Font();
	init_keyboard();
	enable_shell();
	
	/**打印内核信息*/
	printk("Explorer 0.84\n");
	
	/**打印内存管理相关信息*/
	output_mem_info(boot_info);
	
	init_time();
	/**display copyright and device etc information*/
	//put_info();
	/*test for C++*/
	//test_C_plus_plus();
	init_hdd();
	init_FAT32();
	init_mouse();
	init_GUI();
	new_task(&task_test, NULL);
	new_task(&task_test, NULL);
	new_task(&task_test, NULL);
	//run("PROGRAM.BIN", NULL, 0);
}

void put_info(void)
{
	printk("Kernel Name:" KERNEL_NAME " ");
	printk(KERNEL_VERSION "\n");
	printk("Kernel Author:" AUTHOR "\n");
	printk(COPYRIGHT "\n");
	if (BASE != NULL) printk("It is based on " BASE ".\n");
	printk(DESCRIPTION "\n\n\n");
	/*show CPU information*/
	output_CPU_info();
}
