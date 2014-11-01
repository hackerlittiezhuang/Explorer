;Copyright 2013-2014 by Explorer Developer.
;made by 迷彩红星<1@GhostBirdOS.org>
;Explorer 系统调用 exec函数
;ExplorerOS/Kernel/process/init_exe_environment.asm
;version:Alpha
;8/10/2014 3:41 PM

;全局函数
extern	thread_union_point
extern	printk
global	init_exe_environment

THREAD_SIZE	equ	8192


;代码区
[section .text]
[bits 32]
;Warning:能自由使用的寄存器只有EAX\ECX\EDX
init_exe_environment:
	;mov		esp,[thread_union_point]
	;add		esp,THREAD_SIZE-1
	jmp		dword[0x10000000+4]
