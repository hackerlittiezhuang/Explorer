	echo off
	title 鬼鸟系统内核编译
	set /a Compile_count=1

:UI
	cls
	echo 版权所有 2013-2014 鬼鸟操作系统开发者。
	echo Copyright 2013-2014 by Ghost Bird OS Developer.
	echo 我们的网站:www.GhostBirdOS.org
	echo 功能号	备注
	echo 1	开始第%Compile_count%次编译、装载并运行
	echo 2	仅编译
	echo 3	将KERNEL装载到Explorer.img中
	echo 4	仅运行
	echo 5	将鬼鸟系统应用开发中所有可运行文件加载至FAT32.vhd中
	echo 6	清除所有中间文件
	echo 7	阅读帮助
	echo 8	返回到新版make
	echo 9	重启命令行
	echo 0	退出
	set /p command="功能选择[1,2,3,4,5,6,7,8,9,0]:"
::功能判断
	if "%command%"=="" goto Compile
	if "%command%"=="1" goto Compile
	if "%command%"=="2" goto Compile
	if "%command%"=="3" goto load_KERNEL
	if "%command%"=="4" goto run
	if "%command%"=="7" goto help
	if "%command%"=="6" goto file_del
	if "%command%"=="8" (start public~1.cmd&exit)
	if "%command%"=="9" %0
	if "%command%"=="0" exit
	echo 命令暂不支持,按任意键返回
	pause > nul
	goto UI
	
:Compile
::编译部分
	echo 正在编译...
	nasm loader\loader.asm -o tmp\loader.bin
	nasm -f elf arch\x86\kernel\_start.asm -o tmp\_start.o
	nasm -f elf arch\x86\io.asm -o tmp\io.o
	nasm -f elf arch\x86\fun_asm.asm -o tmp\fun_asm.o
	nasm -f elf arch\x86\trap\trap_of_processor.asm -o tmp\trap_of_processor.o
	nasm -f elf arch\x86\kernel\i8254_asm.asm -o tmp\i8254_asm.o
	nasm -f elf drivers\init.asm -o tmp\init.o
	nasm -f elf drivers\dev_intr.asm -o tmp\dev_intr.o
	nasm -f elf syscalls\do_syscalls.asm -o tmp\do_syscalls.o
	nasm -f elf process\kernel_thread.asm -o tmp\Creat_kernel_thread.o
	nasm -f elf process\do_fork.asm -o tmp\do_fork.o
	nasm -f elf process\init_exe_environment.asm -o tmp\init_exe_environment.o
	nasm -f elf process\Creat_process.asm -o tmp\Creat_process.o
	gcc -c arch\x86\kernel\kernel_start.c -o tmp\kernel_start.o -I "%cd%\include"
	gcc -c arch\x86\kernel\i8254.c -o tmp\i8254.o -I "%cd%\include"
	gcc -c init\main.c -o tmp\main.o -I "%cd%\include"
	gcc -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	gcc -c lib\fonts\font.c -o tmp\font.o -I "%cd%\include"
	gcc -c arch\x86\kernel\memory.c -o tmp\memory.o -I "%cd%\include"
	gcc -c arch\x86\kernel\kmalloc.c -o tmp\kmalloc.o -I "%cd%\include"
	gcc -c arch\x86\kernel\do_page_fault.c -o tmp\do_page_fault.o -I "%cd%\include"
	gcc -c process\process.c -o tmp\process.o -I "%cd%\include"
	gcc -c process\exec.c -o tmp\exec.o -I "%cd%\include"
	gcc -c drivers\hdd.c -o tmp\hdd.o -I "%cd%\include"
	gcc -c drivers\video.c -o tmp\video.o -I "%cd%\include"
	gcc -c arch\x86\kernel\i8259.c -o tmp\i8259.o -I "%cd%\include"
	gcc -c drivers\mouse.c -o tmp\mouse.o -I "%cd%\include"
	gcc -c drivers\keyboard.c -o tmp\keyboard.o -I "%cd%\include"
	gcc -c arch\x86\fun_c.c -o tmp\fun_c.o -I "%cd%\include"
	gcc -c arch\x86\trap\trap.c -o tmp\trap.o -I "%cd%\include"
	gcc -c fs\fat32\fat32.c -o tmp\fat32.o -I "%cd%\include"
	gcc -c syscalls\syscalls.c -o tmp\syscalls.o -I "%cd%\include"
	gcc -c GUI\window.c -o tmp\window.o -I "%cd%\include"
	gcc -c GUI\GUI.c -o tmp\GUI.o -I "%cd%\include"
	gcc -c lib\string.c -o tmp\string.o -I "%cd%\include"
	g++ -c C++\test.cpp -o tmp\test.o -I "%cd%\include"
	
	::各个文件链接
	ld -o tmp\kernel.o	-Ttext 0x11000^
	tmp\_start.o tmp\kernel_start.o tmp\main.o^
	tmp\shell.o tmp\font.o^
	tmp\hdd.o tmp\video.o tmp\mouse.o tmp\keyboard.o tmp\i8254.o tmp\i8254_asm.o tmp\dev_intr.o^
	tmp\i8259.o^
	tmp\memory.o tmp\kmalloc.o tmp\do_page_fault.o tmp\fat32.o^
	tmp\process.o tmp\Creat_kernel_thread.o tmp\Creat_process.o tmp\do_fork.o tmp\init_exe_environment.o tmp\exec.o^
	tmp\fun_asm.o tmp\fun_c.o tmp\trap.o tmp\trap_of_processor.o tmp\io.o^
	tmp\syscalls.o tmp\do_syscalls.o^
	tmp\test.o tmp\string.o tmp\GUI.o tmp\window.o tmp\init.o
	objcopy -R .note -R .comment -S -O binary tmp\kernel.o tmp\kernel.bin

	::二进制复制
	copy /B tmp\loader.bin + tmp\kernel.bin KERNEL > nul
	
	echo 编译完成。
	set /a Compile_count=%Compile_count%+1
	if "%command%"=="1" goto load_KERNEL
	echo 任意键返回
	pause > nul
	goto UI
	
:load_KERNEL
	echo 正在启动WinImage...
::将文件拷贝至Explorer.img
	WinImage ..\image\Explorer.img KERNEL /i /h /y
	echo 装载完成。
	if "%command%"=="1" goto run
	goto UI
	
:run
::启动虚拟机
	echo 启动VirtualBox虚拟机...
	VBoxManage.exe startvm "Ghost Bird 0.02"
	goto UI
	
:help
::显示帮助信息
	cls
	echo Ghost Bird OS 编译系统帮助
	echo.
	echo.
	echo 版权所有 2013-2014 鬼鸟操作系统开发者。
	echo Copyright 2013-2014 by Ghost Bird OS Developer.
	echo 无帮助消息
	echo 按任意键返回
	pause > nul
	goto UI

:file_del
	del /a /f /q KERNEL
	del /s /f /q *.o
	del /s /f /q *.bin
	goto UI
	
:init_vironment
	echo 创建虚拟机...
	VBoxManage createvm --name "Ghost Bird 0.02" --register
	
	
	pause
	goto UI
	