global	init
global	int_Slave_8259A
extern	init_mouse
extern	init_keyboard

[section .text]
[bits 32]
init:
	;call	init_8259A
	call	init_keyboard
	call	init_mouse
	call	ready_keyboard
	call	enable_mouse
	sti
	ret
init_8259A:
	cli
		;设置8259A中断控制器
		mov		al,0x11
		out		0x20,al						;ICW1：边沿触发/级联方式
		mov		al,0x20
		out		0x21,al						;ICW2:起始中断向量
		mov		al,0x04
		out		0x21,al						;ICW3:从片级联到IR2
		mov		al,0x01
		out		0x21,al						;ICW4:非总线缓冲，全嵌套，正常EOI
		mov		al,0x11
		out		0xa0,al						;ICW1：边沿触发/级联方式
		mov		al,0x28
		out		0xa1,al						;ICW2:起始中断向量
		mov		al,0x02;0x04
		out		0xa1,al						;ICW3:从片级联到IR2
		mov		al,0x01
		out		0xa1,al						;ICW4:非总线缓冲，全嵌套，正常EOI
		ret
		
;键盘鼠标初始化程序
	
		port_keydat				equ	0x0060
		port_keysta				equ	0x0064
		port_keycmd				equ	0x0064
		keysta_send_notready	equ	0x02
		keycmd_write_mode		equ	0x60
		kbc_mode				equ	0x47

		mouse_ready_loop:
			in		al,port_keysta
			and		al,keysta_send_notready
			cmp		al,0x00
			jnz		mouse_ready_loop_2
			ret
			mouse_ready_loop_2:
				jmp		mouse_ready_loop
			
		ready_keyboard:
			call	mouse_ready_loop
			mov		al,keycmd_write_mode
			out		port_keycmd,al
			call	mouse_ready_loop
			mov		al,kbc_mode
			out		port_keydat,al
			ret
			
			
		keycmd_sendto_mouse		equ	0xd4
		mousecmd_enable			equ	0xf4

		enable_mouse:
			call	mouse_ready_loop
			mov		al,keycmd_sendto_mouse
			out		port_keycmd,al
			call	mouse_ready_loop
			mov		al,mousecmd_enable
			out		port_keydat,al
			ret
			
int_Slave_8259A:
		mov		al,20h
		out		20h,al
		out		0xa0,al
		iretd