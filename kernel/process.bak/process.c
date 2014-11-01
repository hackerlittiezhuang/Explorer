/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by 迷彩红星<1@GhostBirdOS.org>
 *Explorer 进程管理代码
 *ExplorerOS/Kernel/process/peocess.c
 *version:Alpha
 *7/29/2014 4:58 PM
 */

#include "../include/process.h"
#include "../include/main.h"
#include "../include/macro.h"
#include "../include/memory.h"
#include "../include/HAL/x86/function.h"

/*初始化进程，目前有个神秘错误导致复制子进程不能返回上一函数*/
void init_process(void)
{
	/*初始化kernel_thread枚举中的thread_info结构体的一些信息*/
	/*esp0由时钟中断写*/
	kernel_thread.thread_info.cr3 = pdt;
	kernel_thread.thread_info.ppid = 0;
	kernel_thread.thread_info.next_thread_union = &kernel_thread;
	kernel_thread.thread_info.pid = 0;
	kernel_thread.thread_info.thread_id = 0;
	kernel_thread.thread_info.count = 5;
	kernel_thread.thread_info.default_count = 5;
	/*将thread_union_point指向kernel_thread*/
	thread_union_point = &kernel_thread;
	/*设定创建进程id起始值*/
	creat_process_id = 1;
	/*设定创建线程id起始值*/
	creat_thread_id = 1;
	/*初始化时钟*/	
	init_timer();
}

u32 get_tid(void)
{
	return (*thread_union_point).thread_info.thread_id;
}

u32 get_ppid(void)
{
	return (*thread_union_point).thread_info.ppid;
}

u32 get_pid(void)
{
	return (*thread_union_point).thread_info.pid;
}

void *creat_empty_process(void)
{
	return kmalloc(sizeof(union thread_union));
}
