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
 * Explorer �ں˽��̹���
 * Lab Explorer Developers<2322869088@qq.com>
 * ���ö༶���ȷ��������㷨
 */


unsigned long temp_stack;/*��ʱ��ջָ���ű���*/


/**���У�ʵʱ���ߡ������������ȡ���ʬ���ȴ���
 * ����ʹ�ö༶���������㷨��
 */
static union task *realtime, *high, *normal, *low, *zomble, *wait;

/**����0�������嶨��*/
union task task_0;

/**��ʼ���������*/
void init_task(void)
{
	/**��0��������ݽṹ���и�ֵ*/
	task_0.info.counter = 50;		/**��0�������50΢���ʱ��Ƭ*/
	task_0.info.time_limit = 50;	/**0����ʱ��Ƭ����Ϊ50΢��*/
	task_0.info.runtime = 0;		/**0�����ܹ�����ʱ���0*/
	task_0.info.state = TASK_HIGH;	/**����0����Ϊ�����ȼ�����*/
	task_0.info.msg_list = NULL;	/**�����Ϣ����*/
	
	/**��0��������˫��ѭ������*/
	task_0.info.next = &task_0;
	task_0.info.prev = &task_0;
	
	/**��ǰ����ָ��0����*/
	current = &task_0;
}


/**�����ں˼��̺߳���*/
task_id new_task(int (*function)(void *), void *argument)
{	
	/**Ϊ����������ݽṹ����ռ�*/
	union task *new_task;
	new_task = vmalloc(sizeof(union task));
	if (new_task == NULL) return 0;
	memset(new_task, 0, sizeof(union task));
	
	/**������������ݽṹ���и�ֵ*/
	new_task->info.father = current;		/**���ø�����ָ��*/
	new_task->info.time_limit = 50;			/**ʱ��Ƭ*/
	new_task->info.counter = 0;				/**����ʱ������ʱ��Ƭ*/
	new_task->info.runtime = 0;				/**�������ܹ�ִ��ʱ���0*/
	new_task->info.state = TASK_HIGH;		/**����������Ϊ�����ȼ�����*/
	new_task->info.msg_list = NULL;			/**�����Ϣ����*/
	
	/**��������Ķ�ջ���г�ʼ��*/
	new_task->info.stack = init_Kernel_Task(((unsigned long)new_task + TASK_SIZE), function, argument);
	
	/**����������뵽�����б���*/
	(*(*current).info.next).info.prev = new_task;
	new_task->info.prev = current;
	new_task->info.next = (*current).info.next;
	(*current).info.next = new_task;
	
	/**����*/
	return new_task;
}


/**��������id*/
task_id get_id(void)
{
	return current;
}

/**��ø������id*/
task_id get_pid(void)
{
	return current->info.father;;
}


/**�˳�����*/
void exit(int code)
{
	union task *next, *prev;
	next = (*current).info.next;
	prev = (*current).info.prev;
	
	(*next).info.prev = prev;
	(*prev).info.next = next;
	
	(*current).info.counter = 0;
	printk("Task %d is stopped.\n", current);
finish:
	goto finish;
}

/**�����л�������ʹ���㷨ѡ���¸����񲢼���*/
void schedule(void)
{
	/**ѡ����һ��Ҫ���еĽ���*/
	union task *new_task;
	new_task = current->info.next;
	/**����ʱ��Ƭ*/
	new_task->info.counter = new_task->info.time_limit;
	switch_to(new_task);
	/**�л���������*/
	switch_to(new_task);
}


/**���������*/
void sleep(void)
{
	
}