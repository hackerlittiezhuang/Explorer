/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer task system
 * Explorer/arch/x86/include/task.h
 * version:Alpha
 * 7/29/2014 4:59 PM
 */

#ifndef TASK_H_
#define TASK_H_

#include <types.h>

#define		TASK_STACK_SIZE 8192
#define		MAX_TASK 16

typedef unsigned long int task_id;

struct task_resource
{
};

struct task
{
	unsigned long count;/*运行计时*/
	void *stack;/*堆栈指针*/
	task_id id;/*任务id*/
	u32 default_count;
	char attr;/*属性*/
	struct task_resource *resource;/*任务资源*/
};

/**attr属性定义*/
#define		EMPTY 0
#define		STANDARD 1

#endif
