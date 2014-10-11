/*
 *Copyright 2013-2014 by 2013-2014 by Explorer OS Developer. All rights reserved.
 *made by 迷彩红星<1@GhostBirdOS.org>
 *Explorer 0.01 trap.c头
 *Explorer/Kernel/include/HAL/x86/trap.h
 *7/26/2014 6:33 PM
 */

#ifndef TRAP_H_
#define TRAP_H_

#include <types.h>

//中断描述符表属性
#define IDT_P		0x8000
#define IDT_DPL_0	0x00
#define IDT_DPL_1	0x2000
#define IDT_DPL_2	0x4000
#define IDT_DPL_3	0x6000
#define IDT_16		0x00
#define IDT_32		0x800
#define trap_gate		0x700
#define interrupt_gate	0x600

u32 IDT_len, *IDT_addr, IDT_size;

//void init_trap(void);
void init_trap_of_processor(void);

extern int_DE;
extern int_BP;
extern int_OF;
extern int_DF;
extern int_UD;
extern int_TS;
extern int_GP;
extern int_PF;

void int_Divide_error(void);

/*save all the register which maybe change*/

#define SAVE_ALL
asm(" \
	cld; \
	pushl	%es; \
	pushl	%ds; \
	pushl	%eax; \
	pushl	%ebp; \
	pushl	%edi; \
	pushl	%esi; \
	pushl	%edx; \
	pushl	%ecx; \
	pushl	%ebx; \
	movl	%ss,%edx; \
	movl	%edx,%ds; \
	movl	%edx,%es; \
");

#define RESTORE_ALL
asm(" \
	popl	%ebx; \
	popl	%ecx; \
	popl	%edx; \
	popl	%esi; \
	popl	%edi; \
	popl	%ebp; \
	popl	%eax; \
	popl	%ds; \
	popl	%es; \
	addl	$4,%esp; \
	iret; \
");

#endif
