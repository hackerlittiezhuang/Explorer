/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer function kmalloc
 * Explorer/arch/x86/kernel/kmalloc.c
 * version:Alpha
 * 1/10/2014 6:00 PM
 */

/**
 * 内核核心功能 内存管理 kmalloc函数实现代码
 * 内存分配原理：维护多个内存池，按需求从大小
 * 相近的内存池中获取需要的内存。当相近的内存池
 * 已经满了，则向更高一级的大内存池索取内存，分
 * 成相同大小的两部分，加入到当前所需内存池中，
 * 供使用，如果最大一级内存池缺少内存，则获取页。
 */

#include <kmalloc.h>
#include <stddef.h>
#include <stdlib.h>

struct mem_pool mem_pool;

void init_kmalloc(void)
{
	/**对所有表项设置为“未使用”，防止内存残留数据对表项作出干扰*/
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
	/**内存池统计初始化*/
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
	/**暂时只分配128KB和64KB内存*/
	if (size <= (64 * 128))
	{
		/**如果没有足够的内存，先准备*/
		if (mem_pool.available_64KB == 0)
		{
			/**如果没有可分配的64KB内存却内存池已经满，则拒绝分配*/
			if (mem_pool.num_64KB > (NUMBER_OF_64KB - 2)) return NULL;
			point = kmalloc(128 * 1024);
			/*这里需要判断内存是否存在128KB分配失败*/
			if (point == NULL) return NULL;
			alloc_128KB_to_64KB(point);
		}
		/**到这里意味着有了足够的内存，可以分配*/
		for (n = 0; n < NUMBER_OF_64KB; n ++)
		{
			if (mem_pool.mem_64KB[n].attr == AVAILABLE_TABLE)
			{
				mem_pool.mem_64KB[n].attr = USED_TABLE;
				mem_pool.available_64KB --;
				return mem_pool.mem_64KB[n].base;
			}
		}
	
	
	/*128KB分配*/
	}else{
		/**如果没有足够的内存，先准备*/
		if (mem_pool.available_128KB == 0)/*如果没有*/
		{
			/**如果没有可分配的128KB内存却内存池已经满，则拒绝分配*/
			if (mem_pool.num_128KB == NUMBER_OF_128KB) return NULL;
			/**如果没有可分配的128KB内存但内存池有空间，则申请页来作为新的空间*/
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
		/*如果到这里，一定有128KB的未分配内存*/
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
	/**如果没有可分配的64KB内存却内存池已经满，则拒绝分配*/
	if (mem_pool.num_64KB > (NUMBER_OF_64KB - 2)) return -1;
	/**如果没有可分配的64KB内存但内存池有空间，
	 * 则申请128KB来作为新的空间
	 * 将128KB分割成两个64KB加入64KB内存池中
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
	/**修改内存池信息*/
	mem_pool.num_64KB += 2;
	mem_pool.available_64KB += 2;
	return 0;
}
