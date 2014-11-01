;Copyright 2013-2014 by Explorer Developer.
;made by 迷彩红星<1@GhostBirdOS.org>
;Explorer 进程管理 do_creat_thread函数
;ExplorerOS/Kernel/process/do_creat_thread.asm
;version:Alpha
;8/9/2014 2:09 PM

;全局函数
global	do_creat_thread



;代码区
[section .text]
[bits 32]
;Warning:能自由使用的寄存器只有EAX\ECX\EDX
;do_creat_thread(void *point)
;未完成
do_creat_thread:
	;创建一个新PCB(eax=新进程PCB首地址)
	;call	creat_empty_process
	;数据指针指向PCB
	;mov		ecx,[thread_union_point]
	;获取cr3
	mov		edx,[esi+12]
	ret
