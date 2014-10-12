org		0x7c00
mov		ah,0x42
mov		dl,0x80
mov		bx,cs
mov		ds,bx
mov		si,Disk_Address_Packet
int		0x13
mov		ah,0x0e
;mov		al,'y';[cs:0x7e00]
mov		al,[cs:0x7e00]
int		0x10
jmp		$

Disk_Address_Packet:
	db	Disk_Address_Packet.end - Disk_Address_Packet
	db	0
	db	1
	db	0
	dw	0x7e00
	dw	0
	dq	0
.end:

string:	db	"Hello",0x00
times	510-($-$$)	db	0
dw		0xaa55