/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer task system
 * Explorer/arch/x86/kernel/task/task.c
 * version:Alpha
 * 10/4/2014 10:00 AM
 */

#include <memory.h>
#include <lib/mem.h>
#include <stdlib.h>
#include <spinlock.h>
#include <task.h>

/**
 * Explorer 内核进程管理
 * Lab Explorer Developers<2322869088@qq.com>
 * 采用多级优先反馈队列算法
 */


unsigned long temp_stack;/*临时堆栈指针存放变量*/


/**队列：实时、高、正常、低优先、僵尸、等待，
 * 可以使用多级反馈调度算法。
 */
static union task *realtime, *high, *normal, *low, *zomble, *wait;

/**任务0的联合体定义*/
union task task_0;

/**初始化任务管理*/
void init_task(void)
{
	/**对0任务的数据结构进行赋值*/
	task_0.info.counter = 50;		/**给0任务分配50微秒的时间片*/
	task_0.info.time_limit = 50;	/**0任务时间片长度为50微秒*/
	task_0.info.runtime = 0;		/**0任务总共运行时间归0*/
	task_0.info.state = TASK_HIGH;	/**设置0任务为高优先级任务*/
	task_0.info.msg_list = NULL;	/**清空消息链表*/
	
	/**将0任务做成双向循环链表*/
	task_0.info.next = &task_0;
	task_0.info.prev = &task_0;
	
	/**当前任务指向0任务*/
	current = &task_0;
}


/**创建内核级线程函数*/
task_id new_task(int (*function)(void *), void *argument)
{	
	/**为新任务的数据结构分配空间*/
	union task *new_task;
	new_task = vmalloc(sizeof(union task));
	if (new_task == NULL) return 0;
	memset(new_task, 0, sizeof(union task));
	
	/**对新任务的数据结构进行赋值*/
	new_task->info.father = current;		/**设置父任务指针*/
	new_task->info.time_limit = 50;			/**时间片*/
	new_task->info.counter = 0;				/**创建时不分配时间片*/
	new_task->info.runtime = 0;				/**新任务总共执行时间归0*/
	new_task->info.state = TASK_HIGH;		/**设置新任务为高优先级任务*/
	new_task->info.msg_list = NULL;			/**清空消息链表*/
	
	/**对新任务的堆栈进行初始化*/
	new_task->info.stack = Init_Kernel_Task(((unsigned long)new_task + TASK_SIZE), function, argument);
	
	/**将新任务插入到任务列表中*/
	(*(*current).info.next).info.prev = new_task;
	new_task->info.prev = current;
	new_task->info.next = (*current).info.next;
	(*current).info.next = new_task;
	
	/**返回*/
	return new_task;
}


/**获得任务的id*/
task_id get_id(void)
{
	return current;
}

/**获得父任务的id*/
task_id get_pid(void)
{
	return current->info.father;;
}


/**退出任务*/
void exit(int code)
{
	/**指向自身的结构体*/
	union task *my = current;
	
	/**将自己从任务链表中剥离*/
	my->info.next->info.prev = my->info.prev;
	my->info.prev->info.next = my->info.next;
	
	
finish:
	goto finish;
}

/**任务切换函数，使用算法选择下个任务并加载*/
void schedule(void)
{
	/**选择下一个要运行的进程*/
	union task *new_task;
	new_task = current->info.next;
	/**分配时间片*/
	new_task->info.counter = new_task->info.time_limit;
	switch_to(new_task);
	/**切换至新任务*/
	switch_to(new_task);
}


/**任务挂起函数*/
void sleep(void)
{
	/**脱离链表*/
	current->info.next->info.prev = current->info.prev;
	current->info.prev->info.next = current->info.next;
	
	/**设置睡眠属性*/
	current->info.state = TASK_SLEEP;
	
	/**调度任务*/
	schedule();
}

/**任务唤醒函数*/
void wakeup(task_id id)
{
	/**判断该任务是否已经挂起*/
	if (id->info.state == TASK_SLEEP)
	{
		/**刚刚唤醒的任务加入正常队列*/
		id->info.state = TASK_HIGH;
		
		/**加入队列*/
		if (normal == NULL)
		{
			/**唯一一个表项*/
			normal = id;
			
			/**做成双向循环链表*/
			id->info.next = id;
			id->info.prev = id;
		}else{
			
			id->info.next = normal->info.next;
			id->info.prev = normal;
			normal->info.next->info.prev = id;
			normal->info.next = id;
		}
	}
	
	/**正常返回*/
	return;
}