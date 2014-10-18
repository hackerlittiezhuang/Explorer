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
 * ������ܣ�intel 8254��Ҫά��ϵͳ����ʱ��Ͷ�ʱ�������
 * ϵͳ��Ӫʱ����΢���ʱ������ϵͳ�õ�ʱ��ĵط������Դ�Ϊ��׼
 * ϵͳ��Ӫʱ�������sys_clock
 *
 * ��ʱ������У��ں˳�����Ե���settimer()����������ʹ�÷���������ע�ͣ�
 * �趨һ����ʱִ�к��������ж�ʱִֻ��һ�κͶ�ʱִ�ж������ѡ��
 
 * ����������ж����Ҫ��ʱִ�о��庯����������Ϊ��ά��
 * ���㣬���������Ҫ�ж�̬����������������Ҫ���Ч�ʵ�ִ��
 * ����������б������������ʽ��������ʱ���С��������
 * ����ʱ���жϷ�������Ҫ����������жϣ������������Ҫ��ִ��
 * �ı����ִ�в����ж϶�ʱ���ԣ�����Ƕ�ʱִֻ��һ�ξͽ���
 * ��������ɾ��������Ƕ�ʱִ�ж�Σ����޸ĵ����ʵ�λ�ã����¼�������
 
 * intel 8254��صı�����������ṩ��
 */

unsigned long long sys_clock;/*ϵͳʱ�䣨΢����㣩*/
static struct timer_task *current_task;/*ָ������ṹ��������ᱻִ�еı�������NULL����˵����������*/

/**
 * ��ʼ��intel 8254оƬ����
 * ��ϵͳʱ��������г�ʼ��
 */
void init_PIT(void)
{
	/*set 0 to sys_clock*/
	sys_clock = 0;
	/*there is nothing task need PIT*/
	current_task = NULL;
	/*register PIT to PIC*/
	register_PIC(0, &int_PIT, "intel 8254");
	/*��ʼ��Ƶ��*/
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, LATCH & 0xff);
	io_out8(PIT_CNT0, LATCH >> 8);
	return;
}

/**
 * intel 8254�ж�ִ�к���
 */
void int_PIT_display(void)
{
	sys_clock++;/*ά��ϵͳʱ�俪��*/
	struct timer_task *task_point;
	task_point = current_task;
	while((task_point != NULL) && ((*task_point).time == sys_clock))
	{
		(*(*task_point).function)();
		if ((*task_point).state == 0)/*����Ƕ�ʱִ�ж�εĶ�ʱ�����񣬾ͼ�������������*/
		{
			settimer((*task_point).function, (*task_point).time_size, (*task_point).state);
		}
		current_task = (*task_point).next_task;
		oldkfree(task_point);
		task_point = (*task_point).next_task;
	}
/*EOI������8259A���������ж�*/
EOI:
	io_out8(0x20, 0x20);
	io_out8(0xA0, 0x20);
}

/**
 * ���ö�ʱ������
 * ����void (*function)(void)Ϊ������ָ�루ָ����Ҫִ�еĵط���
 * timeΪ��ʱ���������Ƕ�ʱִֻ��һ�Σ��ͻ���time΢���ִ�иú���
 * ����Ƕ�ʱִ�ж�Σ��ͻ���time΢���ִ�иú��������Ҹú���ÿ��time΢�����ٴ�ִ��
 * stateΪ��ʱ���ԣ���0����ʱִ�ж�Σ�������ֵ����ʱִֻ��һ�Σ�
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
 * ������ܣ�intel 8254��Ҫά��ϵͳ����ʱ��Ͷ�ʱ�������
 * ϵͳ��Ӫʱ����΢���ʱ������ϵͳ�õ�ʱ��ĵط������Դ�Ϊ��׼
 * ϵͳ��Ӫʱ�������sys_clock
 *
 * ��ʱ������У��ں˳�����Ե���settimer()����������ʹ�÷���������ע�ͣ�
 * �趨һ����ʱִ�к��������ж�ʱִֻ��һ�κͶ�ʱִ�ж������ѡ��
 
 * ����������ж����Ҫ��ʱִ�о��庯����������Ϊ��ά��
 * ���㣬���������Ҫ�ж�̬����������������Ҫ���Ч�ʵ�ִ��
 * ����������б������������ʽ��������ʱ���С��������
 * ����ʱ���жϷ�������Ҫ����������жϣ������������Ҫ��ִ��
 * �ı����ִ�в����ж϶�ʱ���ԣ�����Ƕ�ʱִֻ��һ�ξͽ���
 * ��������ɾ��������Ƕ�ʱִ�ж�Σ����޸ĵ����ʵ�λ�ã����¼�������
 
 * intel 8254��صı�����������ṩ��
 */

unsigned long long sys_clock;/*ϵͳʱ�䣨΢����㣩*/
static struct timer_task *current_task;/*ָ������ṹ��������ᱻִ�еı�������NULL����˵����������*/

/**
 * ��ʼ��intel 8254оƬ����
 * ��ϵͳʱ��������г�ʼ��
 */
void init_PIT(void)
{
	/*set 0 to sys_clock*/
	sys_clock = 0;
	/*there is nothing task need PIT*/
	current_task = NULL;
	/*register PIT to PIC*/
	register_PIC(0, &int_PIT, "intel 8254");
	/*��ʼ��Ƶ��*/
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, LATCH & 0xff);
	io_out8(PIT_CNT0, LATCH >> 8);
	return;
}

/**
 * intel 8254�ж�ִ�к���
 */
void int_PIT_display(void)
{
	sys_clock++;/*ά��ϵͳʱ�俪��*/
	struct timer_task *task_point;
	task_point = current_task;
	while((task_point != NULL) && ((*task_point).time == sys_clock))
	{
		(*(*task_point).function)();
		if ((*task_point).state == 0)/*����Ƕ�ʱִ�ж�εĶ�ʱ�����񣬾ͼ�������������*/
		{
			settimer((*task_point).function, (*task_point).time_size, (*task_point).state);
		}
		current_task = (*task_point).next_task;
		oldkfree(task_point);
		task_point = (*task_point).next_task;
	}
/*EOI������8259A���������ж�*/
EOI:
	io_out8(0x20, 0x20);
	io_out8(0xA0, 0x20);
}

/**
 * ���ö�ʱ������
 * ����void (*function)(void)Ϊ������ָ�루ָ����Ҫִ�еĵط���
 * timeΪ��ʱ���������Ƕ�ʱִֻ��һ�Σ��ͻ���time΢���ִ�иú���
 * ����Ƕ�ʱִ�ж�Σ��ͻ���time΢���ִ�иú��������Ҹú���ÿ��time΢�����ٴ�ִ��
 * stateΪ��ʱ���ԣ���0����ʱִ�ж�Σ�������ֵ����ʱִֻ��һ�Σ�
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
