/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer function kmalloc
 * Explorer/arch/x86/kernel/kmalloc.c
 * version:Alpha
 * 1/10/2014 6:00 PM
 */

/**
 * �ں˺��Ĺ��� �ڴ���� kmalloc����ʵ�ִ���
 * �ڴ����ԭ��ά������ڴ�أ�������Ӵ�С
 * ������ڴ���л�ȡ��Ҫ���ڴ档��������ڴ��
 * �Ѿ����ˣ��������һ���Ĵ��ڴ����ȡ�ڴ棬��
 * ����ͬ��С�������֣����뵽��ǰ�����ڴ���У�
 * ��ʹ�ã�������һ���ڴ��ȱ���ڴ棬���ȡҳ��
 */

#include <kmalloc.h>
#include <stddef.h>
#include <stdlib.h>

struct mem_pool mem_pool;

void init_kmalloc(void)
{
	/**�����б�������Ϊ��δʹ�á�����ֹ�ڴ�������ݶԱ�����������*/
	unsigned long point;
	for (point = 0; point < NUMBER_OF_128KB; point ++) mem_pool.mem_128KB[point].attr = 0;
	for (point = 0; point < NUMBER_OF_64KB; point ++) mem_pool.mem_64KB[point].attr = 0;
	for (point = 0; point < NUMBER_OF_32KB; point ++) mem_pool.mem_32KB[point].attr = 0;
	for (point = 0; point < NUMBER_OF_16KB; point ++) mem_pool.mem_16KB[point].attr = 0;
	for (point = 0; point < NUMBER_OF_8KB; point ++) mem_pool.mem_8KB[point].attr = 0;
	for (point = 0; point < NUMBER_OF_4KB; point ++) mem_pool.mem_4KB[point].attr = 0;
	for (point = 0; point < NUMBER_OF_2KB; point ++) mem_pool.mem_2KB[point].attr = 0;
	for (point = 0; point < NUMBER_OF_1KB; point ++) mem_pool.mem_1KB[point].attr = 0;
	for (point = 0; point < NUMBER_OF_512B; point ++) mem_pool.mem_512B[point].attr = 0;
	for (point = 0; point < NUMBER_OF_256B; point ++) mem_pool.mem_256B[point].attr = 0;
	for (point = 0; point < NUMBER_OF_128B; point ++) mem_pool.mem_128B[point].attr = 0;
	for (point = 0; point < NUMBER_OF_64B; point ++) mem_pool.mem_64B[point].attr = 0;
	for (point = 0; point < NUMBER_OF_32B; point ++) mem_pool.mem_32B[point].attr = 0;
	for (point = 0; point < NUMBER_OF_16B; point ++) mem_pool.mem_16B[point].attr = 0;
	/**�ڴ��ͳ�Ƴ�ʼ��*/
	mem_pool.num_128KB = 0;
	mem_pool.num_64KB = 0;
	mem_pool.available_128KB = 0;
	mem_pool.available_64KB = 0;
}

void *kmalloc(size_t size)
{
	unsigned long n;
	void *retval;

	/**�Դ���Ĳ�����ֵ�����жϣ�kmallocֻʵ�ֶ�1B~128KB���ڴ����*/
	if (size > (128 * 1024)) return NULL;
	
	if (size <= (64 * 1024))
	{
	
		/**����Ƿ����ڴ湩���䣬���û���򷵻ؿ�ָ��*/
		if (mem_pool.available_64KB == 0)
		{
			if (alloc_128KB_to_64KB() == 0) return NULL;
		}
		
		/**��64KB�ڴ湩����*/
		for (n = 0; n < NUMBER_OF_64KB; n++)
		{
			if (mem_pool.mem_64KB[n].attr == AVAILABLE_TABLE)
			{
				retval = mem_pool.mem_64KB[n].base;
				mem_pool.mem_64KB[n].attr = USED_TABLE;
				mem_pool.available_64KB --;
				return retval;
			}
		}
	}
	
}

void kfree(void *point)
{
	unsigned long n;
	for (n = 0; n < NUMBER_OF_128KB; n++)
	{
		if (mem_pool.mem_128KB[n].base == point)
		{
			mem_pool.mem_128KB[n].attr = AVAILABLE_TABLE;
			mem_pool.available_128KB ++;
			return;
		}
	}
	
	for (n = 0; n < NUMBER_OF_64KB; n++)
	{
		if (mem_pool.mem_64KB[n].base == point)
		{
			mem_pool.mem_64KB[n].attr = AVAILABLE_TABLE;
			mem_pool.available_64KB ++;
			return;
		}
	}
	return;
}

static unsigned long alloc_128KB_to_64KB(void)
{
	unsigned long n;
	void *point;
	
	/*���64KB�ڴ�����޿ձ��Ͳ�����64KB�ڴ���м������ڴ�*/
	if (mem_pool.num_64KB == NUMBER_OF_64KB) return 0;
	
	/*���128KB�ڴ�����������ÿ��Է�����ڴ�*/
	if (mem_pool.available_128KB == 0)
	{
		if (alloc_mem_to_128KB() == 0) return 0;/*�޿ɷ�����ڴ棬����128KB�ڴ���м������ڴ�*/
	}
	
	/*��128KB�ڴ���в��ҿ����ڴ�*/
	for (n = 0; n < NUMBER_OF_128KB; n++)
	{
		if (mem_pool.mem_128KB[n].attr == AVAILABLE_TABLE)
		{
			point = mem_pool.mem_128KB[n].base;
			mem_pool.mem_128KB[n].attr = EMPTY_TABLE;/*�ñ����*/
			mem_pool.available_128KB --;
			mem_pool.num_128KB --;
			break;
		}
	}
	
	for (n = 0; n < NUMBER_OF_64KB; n++)
	{
		if (mem_pool.mem_64KB[n].attr == EMPTY_TABLE)
		{
			mem_pool.mem_64KB[n].base = point;
			mem_pool.mem_64KB[n].attr = AVAILABLE_TABLE;
			break;
		}
	}
	
	for (; n < NUMBER_OF_64KB; n++)
	{
		if (mem_pool.mem_64KB[n].attr == EMPTY_TABLE)
		{
			mem_pool.mem_64KB[n].base = (point + 65536);
			mem_pool.mem_64KB[n].attr = AVAILABLE_TABLE;
			break;
		}
	}
	mem_pool.available_64KB += 2;
	mem_pool.num_64KB += 2;
	return 2;
}

static unsigned long alloc_mem_to_128KB(void)
{
	/*���128KB�ڴ�ز������ɸ�����ڴ棬�ͷ���0������0����*/
	if (mem_pool.num_128KB == NUMBER_OF_128KB) return 0;
	void *point;
	point = (void *)oldkmalloc((128 * 1024));
	unsigned long n;
	for (n = 0; n < NUMBER_OF_128KB; n++)
	{
		if (mem_pool.mem_128KB[n].attr == EMPTY_TABLE)
		{
			mem_pool.mem_128KB[n].base = point;
			mem_pool.mem_128KB[n].attr = AVAILABLE_TABLE;
			mem_pool.available_128KB ++;
			mem_pool.num_128KB ++;
			break;
		}
	}
	return 1;
}
