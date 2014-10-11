extern	int_keyboard_display
global	int_keyboard
extern	int_mouse_display
global	int_mouse
extern	int_HDC_display
global	int_HDC

[section .text]
[bits 32]
int_keyboard:
	;����������
	pushad
	;�����ں�̬�����ݶ�
	mov		dx,ss
	mov		ds,dx
	mov		es,dx
	;�жϴ�����
	call	int_keyboard_display
	;EOI
	mov		al,20h
	out		20h,al
	out		0xa0,al
	popad
	iretd

int_mouse:
	;����������
	pushad
	in		al,0x60
	;�����ں�̬�����ݶ�
	mov		dx,ss
	mov		ds,dx
	mov		es,dx
	;�жϴ�����
	call	int_mouse_display
	;EOI
	mov		al,20h
	out		20h,al
	out		0xa0,al
	popad
	iretd
	
int_HDC:
	;����������
	pushad
	;�����ں�̬�����ݶ�
	mov		dx,ss
	mov		ds,dx
	mov		es,dx
	;�жϴ�����
	call	int_HDC_display
	;EOI
	mov		al,20h
	out		20h,al
	out		0xa0,al
	popad
	iretd