/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer task system
 * Explorer/arch/x86/include/task.h
 * version:Alpha
 * 7/29/2014 4:59 PM
 */

#ifndef TASK_H_
#define TASK_H_

#include <stddef.h>

union task *current;/*当前任务*/

/**定义一个新类型：task_id，task_id代表某个任务的id，实质上是该任务的联合体*/
typedef union task* task_id;
	
/**任务0的联合体定义*/
union task task_0;
	
/**任务信息结构*/
struct task_info{
	union task *next, *prev;			/**任务结构体之间组成双向链表*/
	unsigned long stack;				/**堆栈指针*/
	task_id father;						/**父任务*/
	unsigned long counter, time_limit;	/**时间片计时和时间片长度*/
	unsigned long runtime;				/**运行时间*/
	int state;							/**任务的属性*/
	struct msg *msg_list;				/**消息链表*/
};

/**其中state是以下其中一种情况*/

#define TASK_INVALID	0			//无效
#define TASK_HIGH		1			//实时任务
#define TASK_SLEEP		8			//睡眠
#define TASK_ZOMBLE		9			//僵死

/**任务联合体的大小*/
#define TASK_SIZE	8192

/**任务联合体*/
union task
{
	struct task_info info;
	unsigned long stack[TASK_SIZE / sizeof(unsigned long)];
};

/**Ghost Bird 应用程序头部结构体*/
#define GBOS_FLAG_MAX	4
struct GBOS_head
{
	char flag[GBOS_FLAG_MAX];
	int (*entry)(void);
};

/**队列管理函数*/
void init_queue(union task *queue);
void add_queue(union task *queue, union task *task);

extern unsigned long init_Kernel_Task(unsigned long stack, int (*function)(void *), void * argument);/*位于\arch\x86\kernel\task\init_stack.asm文件中*/

/**初始化任务函数*/
void init_task(void);
	
/**创建内核任务线程函数*/
task_id new_task(int (*function)(void *), void * argument);

/**运行任务函数*/
task_id run(char *filename, void *arg, int flag);

/**获得当前任务的id*/
task_id get_id(void);

/**获得父任务的id*/
task_id get_pid(void);
	
/**任务结束函数*/
void exit(int code);

/**任务切换函数，使用算法选择下个任务并加载*/
void schedule(void);

/**任务挂起函数*/
void sleep(void);
	
/**任务唤醒函数*/
void wakeup(task_id id);
	
/**exit other task*/
#define	SUCCESS		0
#define	FAIL		-1
long do_exit(task_id id);

/**allocate a id for a new task*/
task_id Alloc_Task_ID(void);
task_id Get_TASK_ID(void);
void Switch_Task(void);

/**切换到目标任务*/
void switch_to(task_id id);

/**part of spin lock*/
/*void spin_lock_init(spinlock_t *lock);
	
void spin_lock(spinlock_t *lock);
void spin_lock_irqsave(spinlock_t *lock, unsigned long flags);
void spin_lock_irq(spinlock_t *lock);
void spin_lock_bh(spinlock_t *lock);

void spin_unlock(spinlock_t *lock);
void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);
void spin_unlock_irq(spinlock_t *lock);
void spin_unlock_bh(spinlock_t *lock);*/

#endif
