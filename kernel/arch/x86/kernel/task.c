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
	/*Ѱ�ҿձ���*/
	unsigned long n;
	for (n = 0; n < MAX_TASK; n ++)
	{
		if (task[n].attr == EMPTY) break;
	}
	/*���û���ҵ��ձ���*/
	if (n == MAX_TASK) return 0;
	/*��������Ѿ��ҵ��ˣ�����Ϊtask[n]*/
	
}

void switch_task(void)
{
	/*Ѱ�ҿ��Ա����е�����*/
	unsigned long n;
	for (n = (current_task + 1);; n++)
	{
		/**
		 * ��Ѱ�ҿ������е�����ʱ���������������Ϊһ���������飬
		 * �ӵ�ǰ����ʼ�������ң����û�к��ʵľʹӿ�ʼ�������ң�
		 * ���������������趨Ϊһ���ܴ���������ڵ������ֺ��٣�
		 * ��ͻ���ɺܴ�������˷ѣ���������Ӧ����˫������ΪĿ��
		 */
		if (n == MAX_TASK) n = 0;
		if (n == current_task) break;
		/**Ѱ�ҵ�һ���ǿյ�����ṹ��ʱ�������л�*/
		if (task[n].attr != EMPTY)
		{
			//void *stack;
			//asm volatile("movl	%esp,(stack);");
			//task[n].stack = stack;
		}
	}
}