;Copyright 2013-2015 by Explorer Developers.
;made by Lab Explorer Developers<1@GhostBirdOS.org>
;Explorer task Initialize stack
;Explorer/arch/x86/kernel/task/init_stack.asm
;version:Alpha
;11/14/2014 9:12 PM

extern	temp_stack			; unsigned long temp_stack;/*临时堆栈指针存放变量，在\arch\x86\kernel\task\task.c中定义*/
extern	exit				; void exit_Task(void);/*销毁线程函数，在\arch\x86\kernel\task\task.c中定义*/
extern	code_0_selector		; u32 code_0_selector;/*0级代码段选择子，在\arch\x86\include\function.h中定义*/
extern	code_3_selector		; u32 code_0_selector;/*0级代码段选择子，在\arch\x86\include\function.h中定义*/
extern	data_3_selector		; u32 code_0_selector;/*0级代码段选择子，在\arch\x86\include\function.h中定义*/
global	init_Kernel_Task	; extern unsigned long init_Kernel_Task(unsigned long stack, int (*function)(void *), void * argument);
global	restart				; 


[section .text]
[bits 32]

; 初始化内核任务堆栈
; dword[ESP+12]是新任务的参数指针
; dword[ESP+8]是任务的函数指针
; dword[ESP+4]是需要初始化的堆栈的栈顶指针
; 返回值eax是已经初始化过的堆栈的栈顶指针
init_Kernel_Task:
	mov		ecx,[esp+8]
	mov		edx,[esp+12]
	mov		[temp_stack],esp		; 保存esp
	mov		esp,[esp+4]				; 切换堆栈到需要初始化堆栈的栈顶

	push	edx						; 参数指针

	push	dword exit				; 销毁线程的指针
	
	push	ecx						; 任务的函数指针
	pushfd
	push	ebp

	;push	dword 1000000000b		; 标志寄存器

	;pushfd							; int n指令自动压入的数据
	;push	dword[code_0_selector]
	;push	ecx
	
	;push	dword 0x00000000		; 进入中断后的错误码
	;push	dword 0x00000000		; 进入中断后的中断号
	;pushad							; 进入中断后的通用寄存器保存
	;push	dword 0x00000000		; 进入中断后的handle调用
	;push	dword return			; 进入中断后的函数调用
	
	mov		eax,esp					; 将已经初始化过的堆栈的栈顶指针作为返回值
	mov		esp,[temp_stack]		; 还原堆栈
	ret
	
restart:
	mov		eax,[data_3_selector]
	or		eax,3
	push	eax
	push	dword 0x20000000
	mov		eax,[code_3_selector]
	or		eax,3
	push	eax
	push	dword .next
	retf

	jmp		$
	cli
	;jmp		.next
	mov		edx,esp
	mov		ecx,[data_3_selector]
	or		ecx,3
	push	ecx						;SS
	push	edx						;ESP
	push	dword 0					;EFLAG
	mov		eax,[code_3_selector]
	or		eax,3
	push	eax						;CS
	push	dword .next				;EIP
	iretd
	
.next:
	mov		eax,[data_3_selector]
	mov		ds,ax
	mov		eax,0xe0000000
.loop:
	mov		byte [ds:eax],0xff
	inc		eax
	jmp		.loop
	jmp		$
	