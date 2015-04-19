/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer memory management head
 * Explorer/arch/x86/include/memory.h
 * version:Alpha
 * 7/9/2014 8:30 AM
 */

#ifndef MEMORY2_H_
#define MEMORY2_H_

#include <main.h>
#include <stddef.h>


/**初始化内存管理单元函数*/
void init_MMU(struct boot_info *boot_info);

/**设置页使用函数*/
void set_phy_page_used(unsigned long ptr);

/**设置页自由函数*/
void set_phy_page_free(unsigned long ptr);

/**设置内核内存字节页图函数*/
void set_ker_bytemap(unsigned long ptr, char flag);

void *vmalloc(size_t size);
void vfree(void *addr);

#endif