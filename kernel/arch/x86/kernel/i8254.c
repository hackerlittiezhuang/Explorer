/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer intel 8254 support
 * Explorer/arch/x86/kernel/i8254.c
 * version:1.2
 * 7/26/2014 5:26 PM
 */

#include<stdlib.h>
#include<task.h>
#include<types.h>
#include "time.h"
#include "cmos.h"
#include "../include/i8254.h"

/**
 * Intel 8254\8253оƬ(Programmable Interrupt Timer��PIT)
 * ������ܣ�Intel 8254��Ҫά��ϵͳ����ʱ��Ͷ�ʱ�������
 * ϵͳ��Ӫʱ����΢���ʱ������ϵͳ�����Դ�Ϊ��׼���ж�ʱ
 * ϵͳ��Ӫʱ�������system_runtime
 *
 * ��ʱ������У��ں˳�����Ե���settimer()����(����ʹ�÷���������ע��)
 * �趨һ����ʱִ�к��������ж�ʱִֻ��һ�κͶ�ʱִ�ж������ѡ��
 
 * ����������ж����Ҫ��ʱִ�о��庯����������Ϊ��ά��
 * ���㣬���������Ҫ�ж�̬����������������Ҫ���Ч�ʵ�ִ��
 * ����������б������������ʽ��������ʱ���С��������
 * ����ʱ���жϷ�������Ҫ����������жϣ������������Ҫ��ִ��
 * �ı����ִ�в����ж϶�ʱ���ԣ�����Ƕ�ʱִֻ��һ�ξͽ���
 * ��������ɾ��������Ƕ�ʱִ�ж�Σ����޸ĵ����ʵ�λ��(���¼�������)
 
 * intel 8254��صı�����������ṩ��
 */

/**ָ������ṹ����ʼ���������NULL����˵����������*/
static struct timer *current_task = NULL;

/**
 * ��ʼ��intel 8254оƬ����
 */
void init_PIT(void)
{	
	/**��Intel 8259Aע��Intel 8254*/
	register_PIC(0, &PIT_handle, "intel 8254");
	
	/**�趨Ƶ��*/
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, LATCH & 0xff);
	io_out8(PIT_CNT0, LATCH >> 8);
	
	/**��������*/
	return;
}

/**BCD��ת����16���Ʒ���*/
#define BCD_HEX(n)	((n >> 4) * 10) + (n & 0xf)

/**
 * intel 8254�ж�ִ�к���
 * ����������а����˶�ʱ�����ִ�С�ʱ���ˢ�¡�������л�
 */
void PIT_handle(void)
{	
	
	/**ˢ��ʱ��*/
	system_runtime ++;			/**ϵͳ����ʱ��*/
	
	/**�жϵ�ǰ�ǲ���999΢��*/
	if (ms == 999)
	{
		/**΢�����*/
		ms = 0;
		
		/**�ж����*/
		if (sec == 59)
		{
			/**�����*/
			sec = 0;
			
			/**�жϷ�*/
			if (min == 59)
			{
				/**�ֹ���*/
				min = 0;
				
				/**ʣ�µ���ʱ�����жϣ�ֱ�Ӵ�CMOS�ж�ȡ*/
				hour = BCD_HEX(read_cmos(CMOS_CUR_HOUR));		/**��ǰʱ*/
				week_day = BCD_HEX(read_cmos(CMOS_WEEK_DAY));	/**һ���е�ǰ��*/
				day = BCD_HEX(read_cmos(CMOS_MON_DAY));			/**һ���е�ǰ��*/
				mon = BCD_HEX(read_cmos(CMOS_CUR_MON));			/**��ǰ��*/
				year = BCD_HEX(read_cmos(CMOS_CUR_YEAR));		/**��ǰ��*/
				century = BCD_HEX(read_cmos(CMOS_CUR_CEN));		/**��ǰ����*/
				/**CMOS�е���ݽ����Ǵӱ����Ϳ�ʼ�����ڵľ���ֵ����Ҫ��������*/
				year += century * 100;
			}else{
				min ++;
			}
		}else{
			sec ++;
		}
	}else{
		ms ++;					/**΢���ʱ*/
	}
	
/**ִ�ж�ʱ����*/
	/**�жϴ�ʱ�Ƿ�����������*/
	if (current_task != NULL)
	{
		/**ִ�ж�ʱ����*/
		struct timer *ptr;
		ptr = current_task;
		
		/**ѭ���ж��Ƿ�������ʱ��Ҫִ��*/
		while((ptr != NULL) && (ptr->time <= system_runtime))
		{
			/**ִ�ж�ʱ����*/
			(*ptr->function)();
			
			/**����Ƕ�ʱִ�ж�εĶ�ʱ�����񣬾ͼ�������������*/
			if (ptr->state == 0)
			{
				settimer((*ptr).function, (*ptr).time_size, (*ptr).state);
			}
			
			/**������Ƕ��ִ�����񣬾ʹ��������Ƴ�*/
			current_task = ptr->next;
			vfree(ptr);
			ptr = ptr->next;
		}
	}
	
	/**EOI������8259A���������ж�*/
	EOI();
	
/**�����л�*/
	current->info.runtime ++;		/**����ʱ���һ*/
	
	/**�жϸ������ʱ��Ƭ�Ƿ�ʹ���꣬ʹ��������ѡ�����������*/
	if ((*current).info.counter == 0)
	{
		schedule();
	}else{
		(*current).info.counter --;
	}
	
	/**����*/
	return;
}

/**
 * ���ö�ʱ������
 * ����void (*function)(void)Ϊ������ָ��(ָ����Ҫִ�еĵط�)
 * timeΪ��ʱ���������Ƕ�ʱִֻ��һ�Σ��ͻ���time΢���ִ�иú���
 * ����Ƕ�ʱִ�ж�Σ��ͻ���time΢���ִ�иú��������Ҹú���ÿ��time΢�����ٴ�ִ��
 * stateΪ��ʱ���ԣ���0(��ʱִ�ж��)������ֵ(��ʱִֻ��һ��)
 */
struct timer *settimer(void (*function)(void), unsigned long time, unsigned char state)
{	
	struct timer *retval, *ptr;
	
	/**�������*/
	if (time == 0) return NULL;
	
	/**�����µĶ�ʱ����ṹ��*/
	retval = (struct timer *) vmalloc(sizeof(struct timer));
	if (retval == NULL) return NULL;
	
	/**�Զ�ʱ����ṹ�帳ֵ*/
	retval->time_size = time;
	retval->time = time + system_runtime;
	retval->state = state;
	retval->function = function;

	/**�ж������Ƿ�Ϊ��*/
	if (current_task == NULL)
	{
		/**ֱ����ΪΨһ����*/
		current_task = retval;
		retval->next = NULL;
		
	/**�ж��������Ƿ��ֻ��һ������*/
	}else if (current_task->next == NULL)
	{
		/**����¶�ʱ�����ʱ��ȸñ���С��˵��������Ҫ�ȸ�����ִ��ʱ����*/
		if (retval->time < current_task->time)
		{
			/**����ǰ��*/
			retval->next = current_task;
			current_task = retval;
		/**������ȸ�����ִ��ʱ����*/
		}else{
			/**�������*/
			current_task->next = retval;
			retval->next = NULL;
		}
	/**�в�ֹһ������*/
	}else{
		/**�ҵ�����λ��*/
		for (ptr = current_task; ; ptr = ptr->next)
		{
			/**����ñ����Ѿ������һ��������˳�Ѱ��*/
			if (ptr->next == NULL) break;
			
			/**����ñ��������±���֮��ִ�У����˳�Ѱ��*/
			if (ptr->next->time > retval->time) break;/**���ܴ�������*/
		}
		
		/**���뵥��������*/
		retval->next = ptr->next;
		ptr->next = retval;
	}
}
