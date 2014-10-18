<<<<<<< HEAD
/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer intel 8254 support
 * Explorer 0.01/arch/x86/include/i8259.h
 * version:1.0
 * 7/26/2014 5:26 PM
 */

#include<stdlib.h>
#include<types.h>
#include "../include/trap.h"
#include "../include/i8254.h"

/**
 * 整体介绍：intel 8254需要维护系统运行时间和定时任务队列
 * 系统运营时间以微秒计时，整个系统用到时间的地方可以以此为基准
 * 系统运营时间变量：sys_clock
 *
 * 定时任务队列：内核程序可以调用settimer()函数（具体使用方法看函数注释）
 * 设定一个定时执行函数任务。有定时只执行一次和定时执行多次两种选择
 
 * 任务队列中有多个需要定时执行具体函数的任务表项，为了维护
 * 方便，任务队列需要有动态增长的能力，且需要最大效率的执行
 * 所以任务队列被做成链表的形式，按唤醒时间从小到大排列
 * 当定时器中断发生后，需要对任务队列判断，如果发现了需要被执行
 * 的表项，就执行并且判断定时属性，如果是定时只执行一次就将其
 * 从链表中删除，如果是定时执行多次，就修改到合适的位置（重新加入链表）
 
 * intel 8254相关的编程资料暂无提供。
 */

unsigned long long sys_clock;/*系统时间（微秒计算）*/
static struct timer_task *current_task;/*指向任务结构链表最近会被执行的表项，如果是NULL，就说明链表不存在*/

/**
 * 初始化intel 8254芯片函数
 * 对系统时间变量进行初始化
 */
void init_PIT(void)
{
	/*set 0 to sys_clock*/
	sys_clock = 0;
	/*there is nothing task need PIT*/
	current_task = NULL;
	/*register PIT to PIC*/
	register_PIC(0, &int_PIT, "intel 8254");
	/*初始化频率*/
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, LATCH & 0xff);
	io_out8(PIT_CNT0, LATCH >> 8);
	return;
}

/**
 * intel 8254中断执行函数
 */
void int_PIT_display(void)
{
	sys_clock++;/*维护系统时间开销*/
	struct timer_task *task_point;
	task_point = current_task;
	while((task_point != NULL) && ((*task_point).time == sys_clock))
	{
		(*(*task_point).function)();
		if ((*task_point).state == 0)/*如果是定时执行多次的定时器任务，就继续插入链表中*/
		{
			settimer((*task_point).function, (*task_point).time_size, (*task_point).state);
		}
		current_task = (*task_point).next_task;
		oldkfree(task_point);
		task_point = (*task_point).next_task;
	}
/*EOI，允许8259A继续接收中断*/
EOI:
	io_out8(0x20, 0x20);
	io_out8(0xA0, 0x20);
}

/**
 * 设置定时器函数
 * 其中void (*function)(void)为函数型指针（指向需要执行的地方）
 * time为定时间隔，如果是定时只执行一次，就会在time微秒后执行该函数
 * 如果是定时执行多次，就会在time微秒后执行该函数，而且该函数每隔time微妙后会再次执行
 * state为定时属性，有0（定时执行多次）和其它值（定时只执行一次）
 */
long settimer(void (*function)(void), unsigned long long time, unsigned char state)
{
	/*argument check*/
	if (time == 0) return -1;
	
	struct timer_task *new_task, *task_point;
	task_point = current_task;
	new_task = (struct timer_task *) oldkmalloc(sizeof(struct timer_task));
	(*new_task).time_size = time;
	(*new_task).time = sys_clock + time;
	(*new_task).state = state;
	(*new_task).function = function;
	if (current_task == NULL)
	{
		current_task = new_task;
		return 0;
	}else{
		while(((*task_point).time < (*new_task).time) && (*task_point).next_task != NULL)
		{
			task_point = (*task_point).next_task;
		}
		(*task_point).next_task = new_task;
	}
}
=======
/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer intel 8254 support
 * Explorer 0.01/arch/x86/include/i8259.h
 * version:1.0
 * 7/26/2014 5:26 PM
 */

#include<stdlib.h>
#include<types.h>
#include "../include/trap.h"
#include "../include/i8254.h"

/**
 * 整体介绍：intel 8254需要维护系统运行时间和定时任务队列
 * 系统运营时间以微秒计时，整个系统用到时间的地方可以以此为基准
 * 系统运营时间变量：sys_clock
 *
 * 定时任务队列：内核程序可以调用settimer()函数（具体使用方法看函数注释）
 * 设定一个定时执行函数任务。有定时只执行一次和定时执行多次两种选择
 
 * 任务队列中有多个需要定时执行具体函数的任务表项，为了维护
 * 方便，任务队列需要有动态增长的能力，且需要最大效率的执行
 * 所以任务队列被做成链表的形式，按唤醒时间从小到大排列
 * 当定时器中断发生后，需要对任务队列判断，如果发现了需要被执行
 * 的表项，就执行并且判断定时属性，如果是定时只执行一次就将其
 * 从链表中删除，如果是定时执行多次，就修改到合适的位置（重新加入链表）
 
 * intel 8254相关的编程资料暂无提供。
 */

unsigned long long sys_clock;/*系统时间（微秒计算）*/
static struct timer_task *current_task;/*指向任务结构链表最近会被执行的表项，如果是NULL，就说明链表不存在*/

/**
 * 初始化intel 8254芯片函数
 * 对系统时间变量进行初始化
 */
void init_PIT(void)
{
	/*set 0 to sys_clock*/
	sys_clock = 0;
	/*there is nothing task need PIT*/
	current_task = NULL;
	/*register PIT to PIC*/
	register_PIC(0, &int_PIT, "intel 8254");
	/*初始化频率*/
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, LATCH & 0xff);
	io_out8(PIT_CNT0, LATCH >> 8);
	return;
}

/**
 * intel 8254中断执行函数
 */
void int_PIT_display(void)
{
	sys_clock++;/*维护系统时间开销*/
	struct timer_task *task_point;
	task_point = current_task;
	while((task_point != NULL) && ((*task_point).time == sys_clock))
	{
		(*(*task_point).function)();
		if ((*task_point).state == 0)/*如果是定时执行多次的定时器任务，就继续插入链表中*/
		{
			settimer((*task_point).function, (*task_point).time_size, (*task_point).state);
		}
		current_task = (*task_point).next_task;
		oldkfree(task_point);
		task_point = (*task_point).next_task;
	}
/*EOI，允许8259A继续接收中断*/
EOI:
	io_out8(0x20, 0x20);
	io_out8(0xA0, 0x20);
}

/**
 * 设置定时器函数
 * 其中void (*function)(void)为函数型指针（指向需要执行的地方）
 * time为定时间隔，如果是定时只执行一次，就会在time微秒后执行该函数
 * 如果是定时执行多次，就会在time微秒后执行该函数，而且该函数每隔time微妙后会再次执行
 * state为定时属性，有0（定时执行多次）和其它值（定时只执行一次）
 */
long settimer(void (*function)(void), unsigned long long time, unsigned char state)
{
	/*argument check*/
	if (time == 0) return -1;
	
	struct timer_task *new_task, *task_point;
	task_point = current_task;
	new_task = (struct timer_task *) oldkmalloc(sizeof(struct timer_task));
	(*new_task).time_size = time;
	(*new_task).time = sys_clock + time;
	(*new_task).state = state;
	(*new_task).function = function;
	if (current_task == NULL)
	{
		current_task = new_task;
		return 0;
	}else{
		while(((*task_point).time < (*new_task).time) && (*task_point).next_task != NULL)
		{
			task_point = (*task_point).next_task;
		}
		(*task_point).next_task = new_task;
	}
}
>>>>>>> explorer/master
