/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer task system
 * Explorer/arch/x86/kernel/task.c
 * version:Alpha
 * 10/4/2014 10:00 AM
 */

#include "../include/task.h"

/**MAX_TASK���������飨��MAX_TASK������*/
struct task task[MAX_TASK];

/**��ǰ����*/
static unsigned long current;


/**idָ�룬Ϊ����������id*/
static unsigned long id_point;


void init_task(void)
{
	/**��������������Ϊ�ձ���*/
	unsigned long n;
	for (n = 0; n < MAX_TASK; n ++) task[n].attr = EMPTY;
	
	/**��idָ����Ϊ0*/
	id_point = 0;
	
	/**�Ե�ǰ���������������еĳ�ʼ��*/
	current = 0;/*ʹ��ǰ����Ϊ0����*/
	task[current].attr = STANDARD;
	
}

/**
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * ��ȡ��task_id����
 * �ú����ǻ�ȡtask_id��Ψһ����
 * task_idֻ����䲻�����
 */
static task_id get_id(void)
{
	return id_point++;
}

/**
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * �������߳�
 */
task_id creat_thread()
{
	/*Ѱ�ҿձ���*/
	unsigned long n;
	for (n = 0; n < MAX_TASK; n ++)
	{
		if (task[n].attr == EMPTY) break;
	}
	/*���û���ҵ��ձ���*/
	if (n == MAX_TASK) return 0;
	/*��������Ѿ��ҵ��ˣ�����Ϊtask[n]*/
	//task[current].stack = 
	//task[n]
}

/**
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * �л�������
 * �ú�����ʱ���ж���ʹ��
 */
void switch_task(void)
{
	/**
	 * �����̼�������
	 */
	task[current].count ++;
	
	
	/**Ѱ�ҿ��Ա����е�����*/
	unsigned long n;
	for (n = (current + 1);; n++)
	{
		/**
		 * ��Ѱ�ҿ������е�����ʱ���������������Ϊһ���������飬
		 * �ӵ�ǰ����ʼ�������ң����û�к��ʵľʹӿ�ʼ�������ң�
		 * ���������������趨Ϊһ���ܴ���������ڵ������ֺ��٣�
		 * ��ͻ���ɺܴ�������˷ѣ���������Ӧ����˫������ΪĿ��
		 */
		if (n == MAX_TASK) n = 0;
		if (n == current) break;
		/**Ѱ�ҵ�һ���ǿյ�����ṹ��ʱ�������л�*/
		if (task[n].attr != EMPTY)
		{
			//void *stack;
			//asm volatile("movl	%esp,(stack);");
			//task[n].stack = stack;
		}
	}
}