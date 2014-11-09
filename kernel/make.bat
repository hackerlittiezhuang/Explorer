@echo off
::  构建程式
::  适配于 Explorer v15
::
::    ROSE编写 Rose<roselyu@163.com>
set comct=0
if not "%1"=="" (set comct=%1)
if not exist tmp (md tmp)
::if not exist private (md private)
set t=Make 0.31 for Windows
set n=Classic Style
goto main
:done
title %t% - 完成

	echo.
	echo.
	echo.
	echo ^[按任意键返回^]
pause > nul
goto main

:error
cls
title %t% - 链接或编译错误
	color 0c
cls
	echo ** 抱歉 链接或编译错误
	echo.
	echo 错误详情:
	type tmp\error.log|more
goto done


::Main
:main
set /a comct=%comct%+1
color 0f
cls
set command=
set debug=
title %t% - %n%
echo 您当前是第%comct%次编译
    	echo Ghostbird's Offical Website: www.GhostBirdOS.org
	echo.
	echo Enter	构建并启动虚拟机(调试)
	echo 1	构建并启动虚拟机(无调试)
	echo.
	echo 2	启动虚拟机
	echo 3	删除临时文件
	echo 4	发布
	ECHO.
	echo 5	关于和帮助
	echo 6	查看升级日志
	echo.
	echo 0	退出
	echo.
set /p command="功能:"
if "%command%"=="" goto all
if "%command%"=="1" goto debug
if "%command%"=="2" goto run
if "%command%"=="3" goto deltmp
if "%command%"=="4" goto publish
if "%command%"=="5" goto about
if "%command%"=="6" goto log
if "%command%"=="0" exit
echo ** 未知命令 任意键返回
pause > nul
goto main
:all
set debug=y
goto compile
:debug
goto compile
:publish
color 0c
cls

echo 你确定要发布吗？(任意键继续)

color f
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo √ 临时文件清理完毕
cd ..\..\..\
cls
svnshell.cmd coci
exit
:Compile
cls
title %t% - 正在编译...
echo ->tmp\error.log
	taskkill /f /im virtualbox.exe >nul 2>nul
	nasm loader\loader.asm -o tmp\loader.bin
	nasm -f elf arch\x86\kernel\_start.asm -o tmp\_start.o
	nasm -f elf arch\x86\io.asm -o tmp\io.o
	nasm -f elf arch\x86\fun_asm.asm -o tmp\fun_asm.o
	nasm -f elf arch\x86\trap\trap_of_processor.asm -o tmp\trap_of_processor.o
	nasm -f elf arch\x86\kernel\i8254_asm.asm -o tmp\i8254_asm.o
	nasm -f elf drivers\init.asm -o tmp\init.o
	nasm -f elf drivers\dev_intr.asm -o tmp\dev_intr.o
	nasm -f elf syscalls\do_syscalls.asm -o tmp\do_syscalls.o
	call pcc.cmd -c arch\x86\kernel\kernel_start.c -o tmp\kernel_start.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\i8254.c -o tmp\i8254.o -I "%cd%\include"
	call pcc.cmd -c init\main.c -o tmp\main.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\vsprintf.c -o tmp\vsprintf.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\font.c -o tmp\font.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\simsun.c -o tmp\simsun.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\standard_font.c -o tmp\standard_font.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\linux_sun.c -o tmp\linux_sun.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\memory.c -o tmp\memory.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\task.c -o tmp\task.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\kmalloc.c -o tmp\kmalloc.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\do_page_fault.c -o tmp\do_page_fault.o -I "%cd%\include"
	call pcc.cmd -c drivers\hdd.c -o tmp\hdd.o -I "%cd%\include"
	call pcc.cmd -c drivers\video.c -o tmp\video.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\i8259.c -o tmp\i8259.o -I "%cd%\include"
	call pcc.cmd -c drivers\mouse.c -o tmp\mouse.o -I "%cd%\include"
	call pcc.cmd -c drivers\keyboard.c -o tmp\keyboard.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\fun_c.c -o tmp\fun_c.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\trap\trap.c -o tmp\trap.o -I "%cd%\include"
	call pcc.cmd -c fs\fat32\fat32.c -o tmp\fat32.o -I "%cd%\include"
	call pcc.cmd -c syscalls\syscalls.c -o tmp\syscalls.o -I "%cd%\include"
	call pcc.cmd -c GUI\window.c -o tmp\window.o -I "%cd%\include"
	call pcc.cmd -c GUI\GUI.c -o tmp\GUI.o -I "%cd%\include"
	call pcc.cmd -c lib\string.c -o tmp\string.o -I "%cd%\include"
	call pcc.cmd -c lib\graphics.c -o tmp\graphics.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	call p++.cmd -c C++\test.cpp -o tmp\test.o -I "%cd%\include"
	::copy /y private\* tmp\*.o >nul 2>nul
cls
echo -^> 链接目标文件...
	::Link
	ld -o tmp\kernel.o	-Ttext 0x11000^
	tmp\_start.o tmp\kernel_start.o tmp\main.o^
	tmp\shell.o tmp\vsprintf.o^
	tmp\font.o tmp\simsun.o tmp\standard_font.o tmp\linux_sun.o^
	tmp\hdd.o tmp\video.o tmp\mouse.o tmp\keyboard.o tmp\i8254.o tmp\i8254_asm.o tmp\dev_intr.o^
	tmp\i8259.o^
	tmp\task.o^
	tmp\memory.o tmp\kmalloc.o tmp\do_page_fault.o tmp\fat32.o^
	tmp\fun_asm.o tmp\fun_c.o tmp\trap.o tmp\trap_of_processor.o tmp\io.o^
	tmp\syscalls.o tmp\do_syscalls.o tmp\graphics.o^
	tmp\test.o tmp\string.o tmp\GUI.o tmp\window.o tmp\init.o 2>>tmp\error.log
	echo.>>tmp\error.log
	echo objcopy's error:>>tmp\error.log
	objcopy -R .note -R .comment -S -O binary tmp\kernel.o tmp\kernel.bin 2>>tmp\error.log
cls
echo -^> 合并可执行文件...
	::Binary Copy
	copy /B tmp\loader.bin + tmp\kernel.bin KERNEL >nul 2>nul
	findstr No "tmp\error.log">nul&&goto Error
	del /f /s /q tmp\error.log >nul 2>nul
cls
title %t% - 正在写入内核
::Write kernel to Explorer.img
	echo √ 编译代码
	echo -^> 写入内核...
	echo ** 启动虚拟机

	WinImage ..\image\Explorer.img KERNEL /i /h /y


:run
cls
::Start Virtual Machine
title %t% - 启动虚拟机...
	echo √ 编译代码
	echo √ 写入内核
	echo -^> 启动虚拟机...
	ping 127.1 /n 1 >nul 2>nul
	taskkill /f /im virtualbox.exe>nul 2>nul
	VBoxManage.exe startvm "Ghost Bird 0.02" >nul
	if %errorlevel%==0 (goto main) else (echo ** 虚拟机未启动)
cls
	echo √ 编译代码
	echo √ 写入内核
	echo √ 启动虚拟机
	goto done

:about

cls
	echo %t% - %n%
    	echo Ghostbird's Offical Website: www.GhostBirdOS.org
	echo.
        if exist about (type about) else (echo Document not found.)
goto done
:log

cls
	echo %t% - %n%
    	echo Ghostbird's Offical Website: www.GhostBirdOS.org
	echo.
	if exist "updates.txt" (notepad "updates.txt"&goto done)
goto done
:deltmp
cls
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo √ 临时文件清理完毕
goto done
:eof