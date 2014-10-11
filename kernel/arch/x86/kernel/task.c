/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer task system
 * Explorer/arch/x86/kernel/task.c
 * version:Alpha
 * 10/4/2014 10:00 AM
 */

#include "../include/task.h"

struct task task[MAX_TASK];

unsigned long current_task;

void init_task(void)
{
	current_task = 0;
}

task_id creat_thread()
{
	/*寻找空表项*/
	unsigned long n;
	for (n = 0; n < MAX_TASK; n ++)
	{
		if (task[n].attr == EMPTY) break;
	}
	/*如果没有找到空表项*/
	if (n == MAX_TASK) return 0;
	/*到这里就已经找到了，表项为task[n]*/
	
}

void switch_task(void)
{
	/*寻找可以被运行的任务*/
	unsigned long n;
	for (n = (current_task + 1);; n++)
	{
		/**
		 * 在寻找可以运行的任务时，将任务数组给作为一个环形数组，
		 * 从当前任务开始往后面找，如果没有合适的就从开始往后面找，
		 * 如果最大任务数被设定为一个很大的数而现在的任务又很少，
		 * 这就会造成很大的性能浪费，所以最终应当以双向链表为目标
		 */
		if (n == MAX_TASK) n = 0;
		if (n == current_task) break;
		/**寻找到一个非空的任务结构体时，进行切换*/
		if (task[n].attr != EMPTY)
		{
			//void *stack;
			//asm volatile("movl	%esp,(stack);");
			//task[n].stack = stack;
		}
	}
}