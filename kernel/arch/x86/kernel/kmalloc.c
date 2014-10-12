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
	/*compare argument if it want to allocate more than 128KB*/
	if (size > (128*1024)) return NULL;
	
	unsigned long n;
	void *point;
	/*if (size <= 16)
	{
		for (n = 0; n < NUMBER_OF_16B; n ++)
		{
			if ()
		}
	}*/
	/**��ʱֻ����128KB��64KB�ڴ�*/
	if (size <= (64 * 128))
	{
		/**���û���㹻���ڴ棬��׼��*/
		if (mem_pool.available_64KB == 0)
		{
			/**���û�пɷ����64KB�ڴ�ȴ�ڴ���Ѿ�������ܾ�����*/
			if (mem_pool.num_64KB > (NUMBER_OF_64KB - 2)) return NULL;
			point = kmalloc(128 * 1024);
			/*������Ҫ�ж��ڴ��Ƿ����128KB����ʧ��*/
			if (point == NULL) return NULL;
			alloc_128KB_to_64KB(point);
		}
		/**��������ζ�������㹻���ڴ棬���Է���*/
		for (n = 0; n < NUMBER_OF_64KB; n ++)
		{
			if (mem_pool.mem_64KB[n].attr == AVAILABLE_TABLE)
			{
				mem_pool.mem_64KB[n].attr = USED_TABLE;
				mem_pool.available_64KB --;
				return mem_pool.mem_64KB[n].base;
			}
		}
	
	
	/*128KB����*/
	}else{
		/**���û���㹻���ڴ棬��׼��*/
		if (mem_pool.available_128KB == 0)/*���û��*/
		{
			/**���û�пɷ����128KB�ڴ�ȴ�ڴ���Ѿ�������ܾ�����*/
			if (mem_pool.num_128KB == NUMBER_OF_128KB) return NULL;
			/**���û�пɷ����128KB�ڴ浫�ڴ���пռ䣬������ҳ����Ϊ�µĿռ�*/
			for (n = 0; n < NUMBER_OF_128KB; n ++)
			{
				if (mem_pool.mem_128KB[n].attr == EMPTY_TABLE)
				{
					mem_pool.mem_128KB[n].base = (void *) oldkmalloc(128 * 1024);
					mem_pool.mem_128KB[n].attr = AVAILABLE_TABLE;
					mem_pool.num_128KB ++;
					mem_pool.available_128KB ++;
				}
			}
		}
		/*��������һ����128KB��δ�����ڴ�*/
			for (n = 0; n < NUMBER_OF_128KB; n ++)
			{
				if (mem_pool.mem_128KB[n].attr == AVAILABLE_TABLE)
				{
					mem_pool.available_128KB --;
					mem_pool.mem_128KB[n].attr = USED_TABLE;
					return mem_pool.mem_128KB[n].base;
				}
			}
		}
	}

void kfree(void *point)
{
}

static unsigned long alloc_128KB_to_64KB(void *point)
{
	/**���û�пɷ����64KB�ڴ�ȴ�ڴ���Ѿ�������ܾ�����*/
	if (mem_pool.num_64KB > (NUMBER_OF_64KB - 2)) return -1;
	/**���û�пɷ����64KB�ڴ浫�ڴ���пռ䣬
	 * ������128KB����Ϊ�µĿռ�
	 * ��128KB�ָ������64KB����64KB�ڴ����
	 */
	unsigned long n;
	for (n = 0; n < NUMBER_OF_64KB; n ++)
	{
		if (mem_pool.mem_64KB[n].attr == EMPTY_TABLE)
		{
			mem_pool.mem_64KB[n].base = point;
			mem_pool.mem_64KB[n].attr = AVAILABLE_TABLE;
		}
	}
	for (n = 0; n < NUMBER_OF_64KB; n ++)
	{
		if (mem_pool.mem_64KB[n].attr == EMPTY_TABLE)
		{
			mem_pool.mem_64KB[n].base = point + (64 * 1024);
			mem_pool.mem_64KB[n].attr = AVAILABLE_TABLE;
		}
	}
	/**�޸��ڴ����Ϣ*/
	mem_pool.num_64KB += 2;
	mem_pool.available_64KB += 2;
	return 0;
}
