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
#include <lib/string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <spinlock.h>
#include <task.h>

/**
 * Explorer �ں˽��̹���
 * Lab Explorer Developers<2322869088@qq.com>
 * ���ö༶���ȷ��������㷨
 */

/**������ȱ�־*/
static bool schedule_flag = false;

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
	struct process_struct *process_0;
	
	/**��0��������ݽṹ���и�ֵ*/
	task_0.info.counter = 60;		/**��0�������50΢���ʱ��Ƭ*/
	task_0.info.time_limit = 60;	/**0����ʱ��Ƭ����Ϊ50΢��*/
	task_0.info.runtime = 0;		/**0�����ܹ�����ʱ���0*/
	task_0.info.state = TASK_HIGH;	/**����0����Ϊ�����ȼ�����*/
	
	/**��0��������˫��ѭ������*/
	task_0.info.next = &task_0;
	task_0.info.prev = &task_0;
	
	/**��ǰ����ָ��0����*/
	current = &task_0;
	
	/**�������0�ṹ��*/
	for (process_0 = NULL; process_0 == NULL; )
		process_0 = kmalloc(sizeof(struct process_struct), 0);
	
	/**��ս���0�ṹ��*/
	memset(process_0, 0, sizeof(struct process_struct));
	
	/**��0����������0����*/
	task_0.info.pptr = process_0;
	
	/**0���̸�ֵ*/
	process_0->cr3 = read_CR3();
	process_0->msg_list = NULL;
	process_0->nthread ++;
	
	schedule_flag = true;
}


/**�����ں˼��̺߳���*/
union task* new_task(int (*function)(), void *argument)
{
	/**Ϊ����������ݽṹ����ռ�*/
	union task *new_task;
	new_task = vmalloc(sizeof(union task));
	if (new_task == NULL) return 0;
	memset(new_task, 0, sizeof(union task));
	
	/**������������ݽṹ���и�ֵ*/
	new_task->info.father = current;			/**���ø�����ָ��*/
	new_task->info.time_limit = 60;				/**ʱ��Ƭ*/
	new_task->info.counter = 0;					/**����ʱ������ʱ��Ƭ*/
	new_task->info.runtime = 0;					/**�������ܹ�ִ��ʱ���0*/
	new_task->info.state = TASK_HIGH;			/**����������Ϊ�����ȼ�����*/
	new_task->info.pptr = current->info.pptr;	/**������ͬ���Ǳ����̵�һ���߳�*/
	new_task->info.pptr->nthread ++;			/**�����̵��߳�������һ*/
	
	/**��������Ķ�ջ���г�ʼ��*/
	new_task->info.stack = Init_Kernel_Task(((unsigned long)new_task + TASK_SIZE), function, argument);
	
	/**����������뵽�����б���*/
	current->info.next->info.prev = new_task;
	new_task->info.prev = current;
	new_task->info.next = (*current).info.next;
	current->info.next = new_task;
	
	/**����*/
	return new_task;
}

/**ִ��Ӧ�ó���������POSIX��׼��*/
int run_exec(void *arg)
{
	struct GBOS_head *head;
	struct exec_file_para *file_para = arg;
	struct process_struct *new_process;
	/**ֹͣ����*/
	disable_schedule();
	
	/**������̽ṹ��*/
	for (new_process = NULL; new_process == NULL; )
		new_process = kmalloc(sizeof(struct process_struct), 0);
	
	/**��ս��̽ṹ��*/
	memset(new_process, 0, sizeof(struct process_struct));
	
	/**����ǰ�����뵱ǰ�����ѹ�*/
	current->info.pptr->nthread --;
	
	/**�����������ɽ���*/
	current->info.pptr = new_process;
	
	/**���̸�ֵ*/
	new_process->cr3 = new_pdt();
	new_process->msg_list = NULL;
	new_process->nthread ++;

	/**�л�ҳĿ¼��*/
	write_CR3(new_process->cr3);
	
	/**�������*/
	enable_schedule();
	
	/**�����ļ�*/
	file_open(file_para->filename, (void *)0x10000000);
	
	/**���սṹ��*/
	kfree(arg);
	
	/**��׼�ļ�ͷ*/
	head = (void *)0x10000000;
	
	/**����Ƿ�����ЧӦ�ó���*/
	if (head->flag[0] != 'G' |
		head->flag[1] != 'B' |
		head->flag[2] != 'O' |
		head->flag[3] != 'S' )
	{
		/**ʧ�ܷ���*/
		return 0;
	}
	
	/**ִ��Ӧ�ó���*/
	head->entry();
}

/**����������*/
union task* run(char *filename, char *para, int flag)
{
	struct exec_file_para *new_arg;
	
	/**����filename�ַ���ָ�벻��ΪNULL*/
	if (filename == NULL) return NULL;
	
	/**����һ��exec_file_para�ṹ��װ��ִ��run_exec�̵߳Ĳ���*/
	for (new_arg = NULL; new_arg == NULL; )
		new_arg = kmalloc(sizeof(struct exec_file_para), 0);
	
	/**���exec_file_para�ṹ���ֹ��ɸ���*/
	memset(new_arg, 0, sizeof(struct exec_file_para));
	
	/**�����ļ�����Ϣ����exec_file_para��*/
	strncpy(new_arg->filename, filename, 256);
	
	/**����в�����ͬʱ��������*/
	if (new_arg->para != NULL)
		strncpy(new_arg->para, para, 256);
	
	/**�������߳�����run_exec*/
	new_task(&run_exec, new_arg);
}

/**��������id*/
union task* get_id(void)
{
	return current;
}

/**��ø������id*/
union task* get_pid(void)
{
	return current->info.father;;
}


/**�˳�����*/
void exit(int code)
{
	/**ָ������Ľṹ��*/
	union task *my = current;
	
	/**���Լ������������а���*/
	my->info.next->info.prev = my->info.prev;
	my->info.prev->info.next = my->info.next;
	
	/**�������Ⱥ���*/
	schedule();
	
finish:
	goto finish;
}

/**������Ⱥ���*/
void enable_schedule(void)
{
	schedule_flag = true;
}

/**��ֹ���Ⱥ���*/
void disable_schedule(void)
{
	schedule_flag = false;
}

/**�л���Ŀ������*/
extern void switch_to(union task* id);

/**�����л�������ʹ���㷨ѡ���¸����񲢼���*/
void schedule(void)
{
	union task *new_task;
	
	/**�ж��Ƿ��������*/
	if (schedule_flag == false) return;
	
	/**ѡ����һ��Ҫ���еĽ���*/
	new_task = current->info.next;
		
	/**����ʱ��Ƭ*/
	new_task->info.counter = new_task->info.time_limit;
	
	
	/**�л�ҳĿ¼��*/
	write_CR3(new_task->info.pptr->cr3);
	
	/**�л���������*/
	switch_to(new_task);
}


/**���������*/
void sleep(void)
{
	/**��������*/
	current->info.next->info.prev = current->info.prev;
	current->info.prev->info.next = current->info.next;
	
	/**����˯������*/
	current->info.state = TASK_SLEEP;
	
	/**��������*/
	schedule();
}

/**�����Ѻ���*/
void wakeup(union task* target)
{
	/**�жϸ������Ƿ��Ѿ�����*/
	if (target->info.state == TASK_SLEEP)
	{
		/**�ոջ��ѵ����������������*/
		target->info.state = TASK_HIGH;
		
		/**���������*/
		target->info.prev = current;
		target->info.next = current->info.next;
		current->info.next = target;
		target->info.next->info.prev = target;
	}
	
	/**��������*/
	return;
}