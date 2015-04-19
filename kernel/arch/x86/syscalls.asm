;Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
;made by Lab Explorer Developers<1@GhostBirdOS.org>
;Explorer x86 syscalls support
;Explorer/arch/x86/syscalls.asm
;2/8/2015 10:23 PM

;Explorer临时系统调用规划
;EAX = 功能号,EBX = 子功能号
;EDX = 相关结构体指针参数
;Explorer开发者建议C、C++语言编写的系统调用执行函数参数以结构体指针形式传递
;Explorer开发者建议汇编语言编写的系统调用执行函数参数以结构体指针或者寄存器形式传递

global do_syscalls				;系统调用处理函数
extern printk					;标准输入输出函数
extern sys_GUI_API				;GUI API函数

%define NUM_SYS_FUNCTION 32		;最多功能数量

;功能列表


;上下文在堆栈中的位置
;	long edi;		/**esp + 4*/
;	long esi;		/**esp + 8*/
;	long ebp;		/**esp + 12*/
;	long esp;		/**esp + 16*/
;	long ebx;		/**esp + 20*/
;	long edx;		/**esp + 24*/
;	long ecx;		/**esp + 28*/
;	long eax;		/**esp + 32*/

[section .data]
	;用于测试上下文在堆栈中精确位置的
	; string	db	"The EAX is %#X,The EBX is %#X.",0x00
	
	;用于测试条件跳转指令
	; string_suc	db	"success to syscall",0x00

	;功能调用指针列表
sys_function_list:
	dd		sys_GUI_API		;0号系统调用
.end	times (NUM_SYS_FUNCTION * 4) - (.end - sys_function_list) dd 0

[section .text]
[bits 32]
do_syscalls:
	;测试上下文在堆栈中的具体位置
	; mov		eax,esp
	; push	dword [eax + 20]
	; push	dword [eax + 32]
	; push	dword string
	; call	printk
	; add		esp,12
	; jmp		$
	
	;判断需要的功能是否超过了功能数量的限制
	cmp		eax,(NUM_SYS_FUNCTION - 1)
	ja		.end
	
	;测试是否成功执行系统调用
	; push	dword string_suc
	; call	printk
	; add		esp,4
	
	;根据EAX从系统调用列表中选择一个功能进行调用
	push	edx						;压入结构体指针参数
	push	ebx						;压入子功能号
	push	dword .finish			;压入返回指针
	mov		eax,[((eax * 4) + sys_function_list)]		;重要：在这里EAX从功能号变成功能指令指针
	
	;判断该功能的指令指针是否为NULL
	cmp		eax,0x0
	jz		.finish
	push	eax
	
	;返回跳转
	ret
	
;完成系统调用
.finish:
	add		esp,8
	mov		[(esp + 32)],eax
	
;结束系统调用
.end:
	ret
	