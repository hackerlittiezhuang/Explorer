;Copyright 2013-2014 by Explorer Developer.
;made by Hu wenjie(CN)<1@GhostBirdOS.org>
;Ghost Bird loader for hard disk definition of disk-related information
;Explorer 0.01/arch/x86/boot/hdd define.inc
;version:Alpha
;8/29/2014 10:22 PM

MBRSEGMENT		EQU 0
MBROFFSET		EQU 0X2600

PARTITION0:
	.STRLBA		EQU MBROFFSET+0X1BE+8