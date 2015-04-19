@echo Copyright 2013-2015 Ghost Bird OS Developers.All rights reserved. 
@echo Ghost Bird OS C programme version - boot.
:make
	@pause
	@nasm "boot(floppy disk).asm" -o "boot(floppy disk).bin"
	@goto make