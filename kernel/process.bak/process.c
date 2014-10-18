/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by �Բʺ���<1@GhostBirdOS.org>
 *Explorer ���̹������
 *ExplorerOS/Kernel/process/peocess.c
 *version:Alpha
 *7/29/2014 4:58 PM
 */

#include "../include/process.h"
#include "../include/main.h"
#include "../include/macro.h"
#include "../include/memory.h"
#include "../include/HAL/x86/function.h"

/*��ʼ�����̣�Ŀǰ�и����ش����¸����ӽ��̲��ܷ�����һ����*/
void init_process(void)
{
	/*��ʼ��kernel_threadö���е�thread_info�ṹ���һЩ��Ϣ*/
	/*esp0��ʱ���ж�д*/
	kernel_thread.thread_info.cr3 = pdt;
	kernel_thread.thread_info.ppid = 0;
	kernel_thread.thread_info.next_thread_union = &kernel_thread;
	kernel_thread.thread_info.pid = 0;
	kernel_thread.thread_info.thread_id = 0;
	kernel_thread.thread_info.count = 5;
	kernel_thread.thread_info.default_count = 5;
	/*��thread_union_pointָ��kernel_thread*/
	thread_union_point = &kernel_thread;
	/*�趨��������id��ʼֵ*/
	creat_process_id = 1;
	/*�趨�����߳�id��ʼֵ*/
	creat_thread_id = 1;
	/*��ʼ��ʱ��*/	
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
