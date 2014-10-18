/*
 *Copyright 2013-2014 by Explorer Developer.
 *made by Hu wenjie(CN)<1@GhostBirdOS.org>
 *Explorer syscalls
 *ExplorerOS/Kernel/syscalls/syscalls.c
 *version:Alpha
 *8/10/2014 1:02 AM
 */

#include "../include/syscalls.h"
#include "../include/arch.h"

void init_syscalls(void)
{
	creat_IDT(0xff, code_0_selector, &do_sys_calls, trap_gate + IDT_32 + IDT_DPL_3 + IDT_P);
}