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
	unsigned long n;
	void *retval;

	/**对传入的参数数值进行判断，kmalloc只实现对1B~128KB的内存分配*/
	if (size > (128 * 1024)) return NULL;
	
	if (size <= (64 * 1024))
	{
	
		/**检查是否有内存供分配，如果没有则返回空指针*/
		if (mem_pool.available_64KB == 0)
		{
			if (alloc_128KB_to_64KB() == 0) return NULL;
		}
		
		/**有64KB内存供分配*/
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
	
	/*如果64KB内存池中无空表，就不能向64KB内存池中加入新内存*/
	if (mem_pool.num_64KB == NUMBER_OF_64KB) return 0;
	
	/*检查128KB内存池中有无正好可以分配的内存*/
	if (mem_pool.available_128KB == 0)
	{
		if (alloc_mem_to_128KB() == 0) return 0;/*无可分配的内存，就向128KB内存池中加入新内存*/
	}
	
	/*在128KB内存池中查找可用内存*/
	for (n = 0; n < NUMBER_OF_128KB; n++)
	{
		if (mem_pool.mem_128KB[n].attr == AVAILABLE_TABLE)
		{
			point = mem_pool.mem_128KB[n].base;
			mem_pool.mem_128KB[n].attr = EMPTY_TABLE;/*该表项空*/
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
	/*如果128KB内存池不能容纳更多的内存，就返回0（分配0个）*/
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
