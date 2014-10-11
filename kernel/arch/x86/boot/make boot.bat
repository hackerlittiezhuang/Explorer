@echo Copyright 2013-2014 Ghost Bird OS Developer.All rights reserved. 
@echo Ghost Bird OS C programme version - boot.
:make
	@pause
	@nasm "boot(floppy disk).asm" -o "boot(floppy disk).bin"
	@goto make