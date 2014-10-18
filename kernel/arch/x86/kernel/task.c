/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer task system
 * Explorer/arch/x86/kernel/task.c
 * version:Alpha
 * 10/4/2014 10:00 AM
 */

#include "../include/task.h"

/**MAX_TASK个任务数组（有MAX_TASK个任务）*/
struct task task[MAX_TASK];

/**当前任务*/
static unsigned long current;


/**id指针，为新任务分配的id*/
static unsigned long id_point;


void init_task(void)
{
	/**将所有任务表项都设为空表项*/
	unsigned long n;
	for (n = 0; n < MAX_TASK; n ++) task[n].attr = EMPTY;
	
	/**将id指针设为0*/
	id_point = 0;
	
	/**对当前任务在任务数组中的初始化*/
	current = 0;/*使当前任务为0任务*/
	task[current].attr = STANDARD;
	
}

/**
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * 获取新task_id函数
 * 该函数是获取task_id的唯一方法
 * task_id只许分配不许回收
 */
static task_id get_id(void)
{
	return id_point++;
}

/**
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * 创建新线程
 */
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
	//task[current].stack = 
	//task[n]
}

/**
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * 切换任务函数
 * 该函数被时钟中断所使用
 */
void switch_task(void)
{
	/**
	 * 将进程计数增加
	 */
	task[current].count ++;
	
	
	/**寻找可以被运行的任务*/
	unsigned long n;
	for (n = (current + 1);; n++)
	{
		/**
		 * 在寻找可以运行的任务时，将任务数组给作为一个环形数组，
		 * 从当前任务开始往后面找，如果没有合适的就从开始往后面找，
		 * 如果最大任务数被设定为一个很大的数而现在的任务又很少，
		 * 这就会造成很大的性能浪费，所以最终应当以双向链表为目标
		 */
		if (n == MAX_TASK) n = 0;
		if (n == current) break;
		/**寻找到一个非空的任务结构体时，进行切换*/
		if (task[n].attr != EMPTY)
		{
			//void *stack;
			//asm volatile("movl	%esp,(stack);");
			//task[n].stack = stack;
		}
	}
}