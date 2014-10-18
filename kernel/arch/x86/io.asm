<<<<<<< HEAD
;Copyright 2013-2014 by 2013-2014 by Explorer OS Developer. All rights reserved.
;made by Rose<roselyu@163.com> Hu wenjie(CN)<1@GhostBirdOS.org>
;Explorer 0.01 x86 I/O instruction support
;Explorer 0.01/arch/x86/io.asm
;7/14/2014 1:03 PM

;输入输出函数
global	io_hlt,io_cli,io_sti			;(在新的硬中断发生之前)停机、关中断、开中断
global	io_read_eflags,io_write_eflags	;获取eflags、写eflags
global	io_in8,io_in16,io_in32			;读端口8位、16位和32位
global	io_out8,io_out16,io_out32		;写端口8位、16位和32位

;代码区
[section .text]
[bits 32]

;输入输出函数
io_hlt:
	hlt
	jmp		io_hlt
io_cli:
	cli
	ret
io_sti:
	sti
	ret
io_read_eflags:
	pushfd
	pop		eax
	ret
io_write_eflags:
	mov		eax,[esp+4]
	push	eax
	popfd
	ret
io_in8:
	mov		edx,[esp+4]
	xor		eax,eax
	in		al,dx
	ret
io_in16:
	mov		edx,[esp+4]
	xor		eax,eax
	in		ax,dx
	ret
io_in32:
	mov		edx,[esp+4]
	in		eax,dx
	ret
io_out8:
	mov		edx,[esp+4]
	mov		al,[esp+8]
	out		dx,al
	ret
io_out16:
	mov		edx,[esp+4]
	mov		eax,[esp+8]
	out		dx,ax
	ret
io_out32:
	mov		edx,[esp+4]
	mov		eax,[esp+8]
	out		dx,eax
=======
;Copyright 2013-2014 by 2013-2014 by Explorer OS Developer. All rights reserved.
;made by Rose<roselyu@163.com> Hu wenjie(CN)<1@GhostBirdOS.org>
;Explorer 0.01 x86 I/O instruction support
;Explorer 0.01/arch/x86/io.asm
;7/14/2014 1:03 PM

;输入输出函数
global	io_hlt,io_cli,io_sti			;(在新的硬中断发生之前)停机、关中断、开中断
global	io_read_eflags,io_write_eflags	;获取eflags、写eflags
global	io_in8,io_in16,io_in32			;读端口8位、16位和32位
global	io_out8,io_out16,io_out32		;写端口8位、16位和32位

;代码区
[section .text]
[bits 32]

;输入输出函数
io_hlt:
	hlt
	jmp		io_hlt
io_cli:
	cli
	ret
io_sti:
	sti
	ret
io_read_eflags:
	pushfd
	pop		eax
	ret
io_write_eflags:
	mov		eax,[esp+4]
	push	eax
	popfd
	ret
io_in8:
	mov		edx,[esp+4]
	xor		eax,eax
	in		al,dx
	ret
io_in16:
	mov		edx,[esp+4]
	xor		eax,eax
	in		ax,dx
	ret
io_in32:
	mov		edx,[esp+4]
	in		eax,dx
	ret
io_out8:
	mov		edx,[esp+4]
	mov		al,[esp+8]
	out		dx,al
	ret
io_out16:
	mov		edx,[esp+4]
	mov		eax,[esp+8]
	out		dx,ax
	ret
io_out32:
	mov		edx,[esp+4]
	mov		eax,[esp+8]
	out		dx,eax
>>>>>>> explorer/master
	ret