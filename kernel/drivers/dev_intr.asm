extern	int_keyboard_display
global	int_keyboard
extern	int_mouse_display
global	int_mouse
extern	int_HDC_display
global	int_HDC
global	init_test

[section .text]
[bits 32]
int_keyboard:
	;保存上下文
	pushad
	;换上内核态的数据段
	mov		dx,ss
	mov		ds,dx
	mov		es,dx
	;中断处理部分
	call	int_keyboard_display
	;EOI
	mov		al,20h
	out		20h,al
	out		0xa0,al
	popad
	iretd

int_mouse:
	;保存上下文
	;pushad
	;in		al,0x60
	;换上内核态的数据段
	;mov		dx,ss
	;mov		ds,dx
	;mov		es,dx
	;中断处理部分
	call	int_mouse_display
	;EOI
	;mov		al,20h
	;out		20h,al
	;out		0xa0,al
	;popad
	iretd
	
int_HDC:
	;保存上下文
	pushad
	;换上内核态的数据段
	mov		dx,ss
	mov		ds,dx
	mov		es,dx
	;中断处理部分
	call	int_HDC_display
	;EOI
	mov		al,20h
	out		20h,al
	out		0xa0,al
	popad
	iretd
	
init_test:
	call	ready_keyboard
	call	enable_mouse
	sti
	ret
	
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