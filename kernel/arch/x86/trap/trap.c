<<<<<<< HEAD
/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by �Բʺ���<1@GhostBirdOS.org>
 *Explorer �ж���ϵͳ
 *ExplorerOS/Kernel/HAL/x86/trap/trap.c
 *version:Alpha
 *7/26/2014 5:22 AM
 */

#include "../include/function.h"
#include "../include/trap.h"
#include "../include/address.h"
#include <types.h>

void init_trap(void)
{
	IDT_len = 256;
	IDT_size = IDT_len * 8;
	IDT_addr = (u32 *) kmalloc(IDT_size);
	write_IDTR((u32)IDT_addr, IDT_size - 1);
	clean_IDT();
	init_trap_of_processor();
}

void init_trap_of_processor(void)
{
	creat_IDT(0, code_0_selector, &int_DE, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(3, code_0_selector, &int_BP, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(4, code_0_selector, &int_OF, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(6, code_0_selector, &int_UD, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(8, code_0_selector, &int_DF, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(10, code_0_selector, &int_TS, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(13, code_0_selector, &int_GP, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(14, code_0_selector, &int_PF, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
}

/*����Ϊx86������Ӳ���ж�*/

void int_Divide_error(void)
{
	printk("Divide error!!!");
	io_hlt();
}

void int_Break_point(void)
{
	printk("Break point!!!");
	io_hlt();
}

void int_Over_flow(void)
{
	printk("Over flow!!!");
	io_hlt();
}

void int_Un_define(void)
{
	printk("Undefine!!!");
	io_hlt();
}

void int_Double_fault(void)
{
	printk("Double fault!!!");
	io_hlt();
}

void int_Invalid_TSS(void)
{
	printk("Invalid TSS!!!");
	io_hlt();
}

void int_General_protection(u32 error_code)
{
	printk("General protection interrupt:error code:0x%X", error_code);
}
=======
/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by �Բʺ���<1@GhostBirdOS.org>
 *Explorer �ж���ϵͳ
 *ExplorerOS/Kernel/HAL/x86/trap/trap.c
 *version:Alpha
 *7/26/2014 5:22 AM
 */

#include "../include/function.h"
#include "../include/trap.h"
#include "../include/address.h"
#include <types.h>

void init_trap(void)
{
	IDT_len = 256;
	IDT_size = IDT_len * 8;
	IDT_addr = (u32 *) kmalloc(IDT_size);
	write_IDTR((u32)IDT_addr, IDT_size - 1);
	clean_IDT();
	init_trap_of_processor();
}

void init_trap_of_processor(void)
{
	creat_IDT(0, code_0_selector, &int_DE, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(3, code_0_selector, &int_BP, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(4, code_0_selector, &int_OF, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(6, code_0_selector, &int_UD, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(8, code_0_selector, &int_DF, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(10, code_0_selector, &int_TS, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(13, code_0_selector, &int_GP, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	creat_IDT(14, code_0_selector, &int_PF, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
}

/*����Ϊx86������Ӳ���ж�*/

void int_Divide_error(void)
{
	printk("Divide error!!!");
	io_hlt();
}

void int_Break_point(void)
{
	printk("Break point!!!");
	io_hlt();
}

void int_Over_flow(void)
{
	printk("Over flow!!!");
	io_hlt();
}

void int_Un_define(void)
{
	printk("Undefine!!!");
	io_hlt();
}

void int_Double_fault(void)
{
	printk("Double fault!!!");
	io_hlt();
}

void int_Invalid_TSS(void)
{
	printk("Invalid TSS!!!");
	io_hlt();
}

void int_General_protection(u32 error_code)
{
	printk("General protection interrupt:error code:0x%X", error_code);
}
>>>>>>> explorer/master
