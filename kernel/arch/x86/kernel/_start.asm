<<<<<<< HEAD
;Copyright 2013-2014 by Explorer Developer.
;made by Hu wenjie(CN)<1@GhostBirdOS.org>
;Explorer function _start
;Explorer/arch/kernel/_start.asm
;version:Alpha
;7/9/2014 8:25 AM

;全局变量
;全局函数
extern	kernel_start
extern	main
global	_start

THREAD_SIZE	equ	8192

;代码区
[section .text]
[bits 32]
;Warning:能自由使用的寄存器只有EAX\ECX\EDX
_start:
	call	kernel_start
	call	main
.finish:
	hlt
	jmp		.finish



;数据区
[section .data]

=======
;Copyright 2013-2014 by Explorer Developer.
;made by Hu wenjie(CN)<1@GhostBirdOS.org>
;Explorer function _start
;Explorer/arch/kernel/_start.asm
;version:Alpha
;7/9/2014 8:25 AM

;全局变量
;全局函数
extern	kernel_start
extern	main
global	_start

THREAD_SIZE	equ	8192

;代码区
[section .text]
[bits 32]
;Warning:能自由使用的寄存器只有EAX\ECX\EDX
_start:
	call	kernel_start
	call	main
.finish:
	hlt
	jmp		.finish



;数据区
[section .data]

>>>>>>> explorer/master
