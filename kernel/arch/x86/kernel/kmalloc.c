/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer function kmalloc
 * Explorer/arch/x86/kernel/kmalloc.c
 * version:Alpha
 * 1/10/2014 6:00 PM:created
 * 10/1/2015 8:15 AM:��д�ڴ����
 */

#include <stdlib.h>
#include <stddef.h>
#include <memory.h>
#include "kmalloc.h"

#define PAGE_SIZE	4096

#define MD_PER_PAGE	4096 / sizeof(struct Memory_Descriptor)


/**ά���ڴ��
 * ע�⣺�ڴ������������ڴ治�ó���4096
 */
#define POOL_SIZE	9
static struct mem_pool mem_pool[POOL_SIZE] =
{
	{16  , 0, NULL},
	{32  , 0, NULL},
	{64  , 0, NULL},
	{128 , 0, NULL},
	{256 , 0, NULL},
	{512 , 0, NULL},
	{1024, 0, NULL},
	{2048, 0, NULL},
	{4096, 0, NULL}
};

/**�����ڴ���������Ķ���*/
static struct Memory_Descriptor *empty = NULL;

/**׼����������ڴ�����������*/
static void prepare_MD(void)
{
	/**����һ����ҳ����������*/
	struct Memory_Descriptor *MD = vmalloc(PAGE_SIZE);
	if (MD == NULL) error("fill pool error!");
	
	/**��ʼ����ҳ*/
	unsigned i;
	for (i = 0; i < MD_PER_PAGE; i ++)
	{
		MD[i].next = &MD[i + 1];
	}
	
	/**����ҳ���������뵽�ڴ����*/
	MD[i - 1].next = empty;
	empty = MD;
}

/**����ڴ�غ���*/
void fill_pool(unsigned long n)
{
	/**�жϿ����ڴ����������Ƿ��*/
	if (empty == NULL) prepare_MD();
	
	/**���һ��ҳ*/
	void *new_page = vmalloc(PAGE_SIZE);
	
	/**�ж��Ƿ��ȡ�ɹ�*/
	if (new_page == NULL) error("No enough memory!");
	
	/**��ȡһ�������ڴ���������*/
	struct Memory_Descriptor *new_MD = empty;
	empty = empty->next;
	
	/**��ȡ�����Ϣ*/
	size_t size = mem_pool[n].size;
	unsigned long number = PAGE_SIZE / size;
	
	/**��ʼ���ڴ�������������ҳ*/
	new_MD->page = new_page;
	new_MD->freeptr = new_page;
	new_MD->refcnt = 0;
	
	/**���ڴ�����������뵽�ڴ����*/
	new_MD->next = mem_pool[n].next;
	mem_pool[n].next = new_MD;
	mem_pool[n].number = number;
}

/**�ں�С���ڴ���亯��
 * ����:size - ������ڴ�鳤��
 * ����ֵ:NULL - ����ʧ��; !NULL - ����ɹ��������ڴ��׵�ַ
 */
void *kmalloc(size_t size, int flags)
{
	void *retval;
	
	/**���������*/
	disable_schedule();
	
	/**�ڴ�������ָ��*/
	struct Memory_Descriptor *point;
	
	/**Ѱ�Һ��ʴ�С���ڴ��*/
	unsigned long n;
	for (n = 0; n < POOL_SIZE; n ++)
	{
		if (mem_pool[n].size >= size)
		{
			/**ִ�е�����˵���Ѿ��ҵ����ʴ�С���ڴ��*/
			
			/**�ж��ڴ�����Ƿ����㹻���ڴ�*/
			if (mem_pool[n].number == 0) fill_pool(n);
			
			/**��ȡ�����ڴ�*/
			retval = mem_pool[n].next->freeptr;
			mem_pool[n].next->refcnt ++;
			mem_pool[n].next->freeptr += mem_pool[n].size;
			mem_pool[n].number --;
			goto finish;
		}
	}
	/**���е�����˵��û���ҵ����ʴ�С���ڴ�أ�
	 * ����ԭ������ǲ���size�������е�����ڴ��
	 * ���Է�����ڴ棬�����ķ���Ӧ��ֱ��ʹ����ҳ
	 * Ϊ��λ���䡢���յĺ�����
	 */
	error("argument is too long!");
	return NULL;
	
finish:
	/**�Ѿ�������ڴ����Ĵ���*/
	/**�������*/
	enable_schedule();
	return retval;
}


/**�ں�С���ڴ��ͷź���
 * ����:point - ָ��Ҫ�ͷŵ��ڴ��׵�ַ(֮ǰkmalloc�����ķ���ֵ)
 */
void kfree(void *point)
{
	void *page;
	unsigned long n;
	struct Memory_Descriptor *MD, *prev = NULL;			/**ָ��ǰ����������һ��������*/
	
	/**�����øÿ��ڴ����ڵ�ҳ��*/
	page = (void *)((unsigned long) point & 0xfffff000);
	
	/**Ѱ��ÿ����С���ڴ��*/
	for (n = 0; n < POOL_SIZE; n ++)
	{
		/**Ѱ��ÿ���ڴ�������*/
		for (MD = mem_pool[n].next; MD != NULL; MD = MD->next)
		{
			if (MD->page == page)
			{
				/**��������ڴ�������*/
				MD->refcnt --;
				
				/**�������ڴ�����������Ϊ0*/
				if (MD->refcnt == 0)
				{
					if (prev != NULL)
					{
					}
				}
			}
			prev = MD;
		}
	}
}
