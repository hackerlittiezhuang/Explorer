/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer system calls Definition
 * Explorer/include/syscalls.h
 * version:Alpha
 * 8/11/2014 12:06 AM:created.
 * 1/18/2015 3:20 AM:change the principle of system calls.
 */

#ifndef SYSCALLS_H_
#define SYSCALLS_H_

/**ϵͳ�����жϺ�*/
#define SYSCALLS_INTERRUPT	50

/**������(ͨ�üĴ���)��ѹ���ջ��˳��*/
struct context
{
	long edi;		/**esp + 4*/
	long esi;		/**esp + 8*/
	long ebp;		/**esp + 12*/
	long esp;		/**esp + 16*/
	long ebx;		/**esp + 20*/
	long edx;		/**esp + 24*/
	long ecx;		/**esp + 28*/
	long eax;		/**esp + 32*/
};

/**����ϵͳ���ú�����λ��syscalls.asm��*/
void do_syscalls(struct context context);

#endif