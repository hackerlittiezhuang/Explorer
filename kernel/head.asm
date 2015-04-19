;/head.asm
;Copyright 2015 Lab Explorers.
;This file is the head of loader program


;The BaseAddrLow and BaseAddrHigh together are the 64 bit BaseAddress of this range. The BaseAddress is the
;physical address of the start of the range being specified.

;The LengthLow and LengthHigh together are the 64 bit Length of this range. The Length is the physical
;contiguous length in bytes of a range being specified.

;The Type field describes the usage of the described address range as defined in the table below.
;Value	Type					Description
;1		AddressRangeMemory		This run is available RAM usable by the operating system.
;2		AddressRangeReserved	This run of addresses is in use or reserved by the system,
;								and must not be used by the operating system.
;Other	Undefined				Undefined - Reserved for future use.  Anyrange of this type must be
;								treated by the OS as if the type returned was AddressRangeReserved.
struc Address_Range_Descriptor_Structure
	.BaseAddrLow	resd 1
	.BaseAddrHigh	resd 1
	.LengthLow		resd 1
	.LengthHigh		resd 1
	.Type			resd 1
endstruc

;VBE Mode
	; vbe_mode	EQU	VIDEO_VMWARE_800_600
	vbe_mode	EQU	VIDEO_1024_768

;VBE Mode list
VIDEO_800_600	EQU 0x4115
VIDEO_1024_768	EQU 0x4118
VIDEO_1280_1024	EQU 0x411b
VIDEO_VMWARE_800_600	EQU	0x4140

[bits 16]
	org		0x10000			;loaded into 0x10000

entry:
	;init for registers
	mov		ax,cs
	mov		ds,ax
	mov		es,ax
	;init for stack
	mov		ss,ax
	mov		sp,STACK_TOP	;stack top pointer to top of the segment 0x0000(0xffff)
	;get memory info
	mov		ebx,0			;if this is the first call,EBX must contain zero.
	mov		ecx,20			;size of ARDS
	mov		edx,0534D4150h	;'SMAP' - Signature to verify correct BIOS revision.
	mov		di,ARDS0		;ES:DI pointer to an ARDS which the BIOS is to fill in.
	mov		eax,0xe820		;Function Code
	int		15h				;INT 15h, AX=E820h - Query System Address Map
	mov		di,ARDS1
	mov		eax,0xe820
	int		15h
	mov		di,ARDS2
	mov		eax,0xe820
	int		15h
	mov		di,ARDS3
	mov		eax,0xe820
	int		15h
	mov		di,ARDS4
	mov		eax,0xe820
	int		15h
	mov		di,ARDS5
	mov		eax,0xe820
	int		15h
	mov		di,ARDS6
	mov		eax,0xe820
	int		15h
	mov		di,ARDS7
	mov		eax,0xe820
	int		15h 
	;get vbe info
	mov		ax,0x4f00
	mov		di,VbeInfoBlock_struc
	int		0x10
	;get mode info
	mov		ax,0x4f01
	mov		cx,vbe_mode
	mov		di,ModeInfoBlock_struc
	int		0x10
	;init for video
	mov		ax,0x4F02		;VBE/set video mode
	mov		bx,vbe_mode		;800*600*24bit
	int		0x10
	;close the interruption
	cli
	;load GDTR
	lgdt	[cs:GDTR]
	;enable A20 line
	in		al,0x92
	or		al,0000_0010B
	out		0x92,al
	;set CR0 bit PE
	mov		eax,cr0
	or		eax,1
	mov		cr0,eax
	;far jump:to clean the cs
	jmp		dword 0x08:.prepare

[bits 32]
;prepare for next step
.prepare:
	;init for registers
	mov		ax,0x10
	mov		ds,ax
	mov		es,ax
	mov		fs,ax
	mov		gs,ax
	;init for stack
	mov		ss,ax
	mov		esp,STACK_TOP	;stack top pointer to top of the segment 0x0000(0xffff)
	;prepare boot_info for next step
	mov		eax,boot_info
	;running body
	jmp		_start
	

;数据区
;boot info
boot_info:
	;boot info header
	db		"EBI",0x00									;启动信息结构特殊标识
	dd		boot_info_end - boot_info					;启动信息结构的长度
	;Address Range DescriptorStructure
	ARDS0 istruc Address_Range_Descriptor_Structure
	iend
	ARDS1 istruc Address_Range_Descriptor_Structure
	iend
	ARDS2 istruc Address_Range_Descriptor_Structure
	iend
	ARDS3 istruc Address_Range_Descriptor_Structure
	iend
	ARDS4 istruc Address_Range_Descriptor_Structure
	iend
	ARDS5 istruc Address_Range_Descriptor_Structure
	iend
	ARDS6 istruc Address_Range_Descriptor_Structure
	iend
	ARDS7 istruc Address_Range_Descriptor_Structure
	iend
	;VBE info
	VbeInfoBlock_struc	times 512 db 0x00
	ModeInfoBlock_struc	times 256 db 0x00
boot_info_end:

;Global Descriptor Table,GDT
GDT:
	;0:void
	dd		0x00000000
	dd		0x00000000
	;1:4GB(flat-mode) code segment 0
	dd		0x0000ffff
	dd		0x00cf9A00
	;2:4GB(flat-mode) data segment 0
	dd		0x0000ffff
	dd		0x00cf9200

;Global Descriptor Table Register,GDTR
GDTR:
	.size	dw	(3 * 8) - 1
	.offset	dd	GDT

stack:
	STACK_SIZE			equ 256
	times	STACK_SIZE	db	0
	STACK_TOP			equ	$
	
;Filled with zeros
	times	4096-($-$$)	db	0


;loader program's body
_start:
	;this function is in /arch/x86/kernel/_start.asm,
	;running in 32-bit Protect-mode.