extern	int_keyboard_display
global	int_keyboard
extern	int_mouse_display
global	int_mouse
extern	int_HDC_display
global	int_HDC

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
	pushad
	in		al,0x60
	;换上内核态的数据段
	mov		dx,ss
	mov		ds,dx
	mov		es,dx
	;中断处理部分
	call	int_mouse_display
	;EOI
	mov		al,20h
	out		20h,al
	out		0xa0,al
	popad
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