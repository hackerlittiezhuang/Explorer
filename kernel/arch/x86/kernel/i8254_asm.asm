;Copyright 2013-2014 by Explorer Developer.
;made by Hu wenjie(CN)<1@GhostBirdOS.org>
;Explorer i8254 support
;Explorer 0.01/arch/x86/kernel/i8259_asm.asm
;version:Alpha
;7/28/2014 4:26 PM


extern	switch_task
extern	int_PIT_display
global	int_PIT



;代码区
[section .text]
[bits 32]
;时钟中断处理程序
int_PIT:
	;保存上下文
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	;换上内核态的数据段
	mov		dx,ss
	mov		ds,dx
	mov		es,dx
	;处理时钟函数
	call	int_PIT_display
	;执行任务切换函数
	call	switch_task
	pop		gs
	pop		fs
	pop		es
	pop		ds
	popad
	iret
	
[section .data]