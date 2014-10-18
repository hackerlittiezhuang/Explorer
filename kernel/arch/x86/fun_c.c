<<<<<<< HEAD
/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer x86平台硬件抽象层
 * ExplorerOS/Kernel/HAL/x86/fun_c.c
 * version:Alpha
 * 7/14/2014 1:06 PM
 */

#include "include/function.h"
#include "include/address.h"
#include <types.h>

//结构初始化
void init_arch(void)
{
	/*准备GDT表*/
	write_GDTR(GDT_addr, GDT_size - 1);
	clean_GDT();
	/*准备数据段和代码段*/
	code_0_selector = creat_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_code_32_non_conforming);
	code_3_selector = creat_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_code_32_conforming);
	data_0_selector = creat_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_data_32);
	data_3_selector = creat_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_data_32);
	init_seg_reg(data_0_selector);
	/*准备TSS(真心不该有TSS):-(*/
	TSS.ss0 = data_0_selector;
	TSS_selector = creat_GDT(((u32)&TSS), sizeof(TSS), GDT_P + GDT_DPL_0 + GDT_TSS_data);
}

/**
 * 进入分页模式函数
 * 分页模式是保护模式下一种可选的模式，对内存进行分页管理
 * 实现分页模式可以
 */
void goto_paging(u32 pdt_addr)
{
	write_CR3(pdt_addr);
	write_CR0(read_CR0() | 0x80000000);
}
=======
/**
 * Copyright 2013-2014 by Explorer Developer.
 * made by Hu wenjie(CN)<1@GhostBirdOS.org>
 * Explorer x86平台硬件抽象层
 * ExplorerOS/Kernel/HAL/x86/fun_c.c
 * version:Alpha
 * 7/14/2014 1:06 PM
 */

#include "include/function.h"
#include "include/address.h"
#include <types.h>

//结构初始化
void init_arch(void)
{
	/*准备GDT表*/
	write_GDTR(GDT_addr, GDT_size - 1);
	clean_GDT();
	/*准备数据段和代码段*/
	code_0_selector = creat_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_code_32_non_conforming);
	code_3_selector = creat_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_code_32_conforming);
	data_0_selector = creat_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_data_32);
	data_3_selector = creat_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_data_32);
	init_seg_reg(data_0_selector);
	/*准备TSS(真心不该有TSS):-(*/
	TSS.ss0 = data_0_selector;
	TSS_selector = creat_GDT(((u32)&TSS), sizeof(TSS), GDT_P + GDT_DPL_0 + GDT_TSS_data);
}

/**
 * 进入分页模式函数
 * 分页模式是保护模式下一种可选的模式，对内存进行分页管理
 * 实现分页模式可以
 */
void goto_paging(u32 pdt_addr)
{
	write_CR3(pdt_addr);
	write_CR0(read_CR0() | 0x80000000);
}
>>>>>>> explorer/master
