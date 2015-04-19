;Copyright 2013-2015 by Explorer Developers.
;made by Lab Explorer Developers<1@GhostBirdOS.org>
;Ghost Bird loader for hard disk defInition of disk-related information
;Explorer 0.01/arch/x86/boot/hdd definition.inc
;version:Alpha
;8/29/2014 10:22 PM

MBRSEGMENT		EQU 0
MBROFFSET		EQU 0X2600

PARTITION0:
	.STRLBA		EQU MBROFFSET+0X1BE+8