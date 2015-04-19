/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Memory Management Unit
 * Explorer/arch/x86/kernel/memory.c
 * version:Alpha
 * 5/7/2014
 */

#include "../include/memory.h"
#include <main.h>
#include <stddef.h>
#include <stdlib.h>

/**内存信息*/
unsigned int all_mem = 0, real_mem = 0;

/**内核区域大小*/
#define RANG_KERNEL_SIZE		268435456

/**内核内存字节页图*/
#define KER_MEM_BYTEMAP_PTR		0x100000
#define KER_MEM_BYTEMAP_SIZE	RANG_KERNEL_SIZE / 4096
unsigned char *ker_mem_bytemap = (unsigned char *) KER_MEM_BYTEMAP_PTR;

/**允许分配回收的内核区域起始位置*/
#define KERNEL_ALLOC_START		KER_MEM_BYTEMAP_PTR + KER_MEM_BYTEMAP_SIZE

/**内核内存字节页图中每项的值的含义*/
#define KER_MEM_BYTEMAP_FREE	0b00000000
#define KER_MEM_BYTEMAP_USED	0b00000001
#define KER_MEM_BYTEMAP_START	0b00000010
#define KER_MEM_BYTEMAP_END		0b00000100

/**物理内存位页图*/
#define PHY_MEM_BITMAP_PTR		0x30000
#define PHY_MEM_BITMAP_SIZE		(4294967296 / 4096) / 8
unsigned int *phy_mem_bitmap = (unsigned int *) PHY_MEM_BITMAP_PTR;

/**初始化内存管理单元函数*/
void init_MMU(struct boot_info *boot_info)
{
	unsigned long n;
	unsigned int BaseAddr, Length;
	
	/**打印信息*/
	//printk("Register memory...\n");
	
	/**初始化物理内存位页图*/
	for (n = 0; n < (PHY_MEM_BITMAP_SIZE / sizeof(unsigned int)); n ++)
	{
		/**1代表已经被占用*/
		phy_mem_bitmap[n] = 0xffffffff;
	}
	
	/**初始化内核内存字节页图*/
	for (n = 0; n < KER_MEM_BYTEMAP_SIZE; n ++)
	{
		ker_mem_bytemap[n] == KER_MEM_BYTEMAP_USED | KER_MEM_BYTEMAP_START | KER_MEM_BYTEMAP_END;
	}
	
	/**循环读取Address Range Descriptor Structure ，
	 * 建立物理内存位页图
	 */
	for (n = 0; n < BOOT_FLAG_MAX; n ++)
	{
		/**总内存计数*/
		all_mem += boot_info->ARDS[n].LengthLow;
		
		/**判断是否是高于4GB的范围*/
		if (boot_info->ARDS[n].BaseAddrHigh != 0) break;
		
		/**4KB对齐*/
		boot_info->ARDS[n].BaseAddrLow = boot_info->ARDS[n].BaseAddrLow & 0xfffff000;
		boot_info->ARDS[n].LengthLow = boot_info->ARDS[n].LengthLow & 0xfffff000;
		
		/**判断该ARDS是否可用*/
		if (boot_info->ARDS[n].Type != ARDS_FREE) continue;
		
		/**判断该ARDS的范围是否为0*/
		if (boot_info->ARDS[n].LengthLow == 0) continue;
		
		/**归纳信息*/
		BaseAddr = boot_info->ARDS[n].BaseAddrLow;
		Length = boot_info->ARDS[n].LengthLow;
		real_mem += Length;
		
		/**打印该范围信息*/
		//printk("available memory:%#x~%#x.\n", BaseAddr, BaseAddr + Length);
		
		/**制作相应的物理内存位页图和内核内存字节页图*/
		while (Length != 0)
		{
			/**设置空闲*/
			set_phy_page_free(BaseAddr);
			
			/**判断是否在内核内存区中*/
			if (BaseAddr < RANG_KERNEL_SIZE)
			{
				set_ker_bytemap(BaseAddr, KER_MEM_BYTEMAP_FREE);
			}
			
			/**计数增加*/
			BaseAddr += 0x1000;
			Length -= 0x1000;
		}
	}
	
	/**打印信息*/
	//printk("Register memory done.\n");
	//printk("Installed memory(RAM):%dMB(%dMB is available).\n", all_mem / 1048576, real_mem / 1048576);
	
	/**少于256MB的情况不能下一步初始化*/
	if (all_mem < 268435456)
	{
		printk("Not enough memory!Please make sure the memory more than 256MB.");
		reset();
	}
	
	/**正常返回*/
	return;
}

/**内存管理相关信息输出函数*/
void output_mem_info(struct boot_info *boot_info)
{
	/**打印内存分布信息*/
	unsigned long n;
	printk("Address Range Descriptor Structure:\nBaseAddrLow   BaseAddrHigh  LengthLow     LengthHigh    Type\n");
	for (n = 0; n < BOOT_ARDS_NUM; n++)
	{
		printk("%#010x    %#010x    %#010x    %#010x    %#010x\n", boot_info->ARDS[n].BaseAddrLow, boot_info->ARDS[n].BaseAddrHigh, boot_info->ARDS[n].LengthLow, boot_info->ARDS[n].LengthHigh, boot_info->ARDS[n].Type);
	}
	
	/**打信息*/
	printk("Installed memory(RAM):%dMB(%dKB is available).\n", all_mem / 1048576, real_mem / 1024);
}

/**设置页使用函数*/
void set_phy_page_used(unsigned long ptr)
{
	phy_mem_bitmap[ptr >> 17] = phy_mem_bitmap[ptr >> 17] | (1 << ((ptr >> 12) & 0b11111));
}

/**设置页自由函数*/
void set_phy_page_free(unsigned long ptr)
{
	phy_mem_bitmap[ptr >> 17] = phy_mem_bitmap[ptr >> 17] & ~(1 << ((ptr >> 12) & 0b11111));
}

/**设置内核内存字节页图函数*/
void set_ker_bytemap(unsigned long ptr, char flag)
{
	ker_mem_bytemap[ptr >> 12] = flag;
}

/**获取一个物理页函数*/
void *get_free_page(void)
{
	
}

/**释放一个物理页*/
void free_page(void *addr)
{
}

/**大块内存分配函数*/
void *vmalloc(size_t size)
{
	/**互斥操作*/
	io_cli();
	
	/**判断长度是否为0*/
	if (size == 0) error("allocate is error!");
	
	unsigned long n = (KERNEL_ALLOC_START >> 12), l;
	
	/**对参数化整*/
	if ((size & 0xfff) != 0) size += 0x1000;
	size = size & 0xfffff000;
	
/**n为正在判断的空闲块的首页，l为正在判断的页*/
	/**寻找足够长的内存区域*/
	for (;;)
	{
		/**寻找到空闲区域*/
		if (ker_mem_bytemap[n] == KER_MEM_BYTEMAP_FREE)
		{
			/**循环检查该空闲块长度*/
			for (l = n;;l ++)
			{
				/**如果这个空闲块到这里为止且不够大小*/
				if (ker_mem_bytemap[l] != KER_MEM_BYTEMAP_FREE)
				{
					/**返回并且从下一个页开始找*/
					n = l + 1;
					break;
				}
				
				/**如果长度符合要求*/
				if ((l - n + 1) == (size >> 12))
				{
					/**长度合适跳转到分配模块*/
					goto allocate;
				}
			}
			
		}
		
		/**寻找下一个内存块*/
		n ++;
		
		/**判断寻找是否超过内核内存区域*/
		if (n >= KER_MEM_BYTEMAP_SIZE)
		{
			/**超过了则分配失败*/
			return NULL;
		}
		
		/**判断剩余内核内存块长度是否满足要求*/
		if ((KER_MEM_BYTEMAP_SIZE - n) < (size >> 12))
		{
			return NULL;
		}
	}
	
/**分配模块，该模块将相关内存块设置为已经使用*/
/**n为可用的自由内存块首地址，l为偏移*/
allocate:
	/**首先对首位进行特别标记*/
	ker_mem_bytemap[n] = KER_MEM_BYTEMAP_START | KER_MEM_BYTEMAP_USED;
	ker_mem_bytemap[n + (size >> 12) - 1] = ker_mem_bytemap[n + (size >> 12) - 1] | KER_MEM_BYTEMAP_END | KER_MEM_BYTEMAP_USED;
	
	/**将相关内存块都标记为占用*/
	for (l = n; (l - n + 1) != (size >> 12); l ++)
	{
		ker_mem_bytemap[l] = ker_mem_bytemap[l] | KER_MEM_BYTEMAP_USED;
	}
	
	/**互斥操作*/
	io_sti();
	
	/**正常返回*/
	return (void *) (n << 12);
}


/**大块内存释放函数*/
void vfree(void *addr)
{
	/**判断该地址是否为这个被分配的内存块的首地址*/
	if ((ker_mem_bytemap[(unsigned long) addr >> 12] & KER_MEM_BYTEMAP_START) != KER_MEM_BYTEMAP_START)
	{
		/**输出错误信息*/
		printk("free the memory is error: this address is not the head of the block\n");
	}
	unsigned long n;
	
	/**循环回收内存块*/
	for (n = ((unsigned long) addr >> 12); ((ker_mem_bytemap[n] & KER_MEM_BYTEMAP_END) != KER_MEM_BYTEMAP_END); n ++)
	{
		/**回收内存块*/
		ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;
	}
	
	/**回收末尾的内存块*/
	ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;
	
	/**正常退出*/
	return;
}

