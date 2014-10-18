@echo off
::::::::::::::::::::::::::::::::::::::
::  构建程式 - Smilodon 助力
::  简体中文版
::  适配于 Explorer v14
::
::    ROSE编写 Rose<roselyu@163.com>
::::::::::::::::::::::::::::::::::::::
chcp 936
::切换到简体中文编码页

if not exist tmp (md tmp)
if not exist private (md private)
set t=构建程式 0.2 中文版
set n=v14系列适配
goto main
:done
<<<<<<< HEAD:kernel/Make.cmd
title %t% - 完成
=======
title %t% - Done
>>>>>>> explorer/master:kernel/Public Make.cmd
	echo.
	echo.
	echo.
	echo ^[按任意键返回^]
pause > nul
goto main

:error
cls
title %t% - 编译错误
	color 0c
cls
	echo ** 抱歉 编译错误
	echo ** 源代码中可能出现一些错误
	echo.
	echo 错误详情:
	type tmp\error.log
goto done


::Main
:main
color 0f
mode con cols=38 lines=30
cls
set command=
set debug=
title %t% - %n%
    	echo 我们的网站 www.GhostBirdOS.org
	echo --------------------------------------
	echo.
	echo +    Enter - 构建并启动虚拟机(调试)
	echo +        1 - 构建并启动虚拟机(快速)
	echo.
	echo +        2 - 启动虚拟机
	echo +        3 - 删除临时文件
	echo +        4 - 发布
	ECHO.
	echo +        5 - 关于和帮助
	echo +        6 - 查看升级日志
	echo.
	echo +        0 - 退出
	echo.
	echo --------------------------------------
	echo [   按下 ENTER 执行构建并启动虚拟机  ]
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
<<<<<<< HEAD:kernel/Make.cmd
echo 你确定要发布吗？(任意键继续)
=======
echo Are you sure to PUBLISH?(Press any key to continue)
>>>>>>> explorer/master:kernel/Public Make.cmd
color f
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo ** 临时文件清理完毕
cd ..\..\..\
svnshell.cmd coci
exit
:Compile
cls
mode con cols=80 lines=25
title %t% - 正在编译...
echo -------------------------------------->tmp\error.log
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
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
	call pcc.cmd -c arch\x86\kernel\task.c -o tmp\task.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\i8254.c -o tmp\i8254.o -I "%cd%\include"
	call pcc.cmd -c init\main.c -o tmp\main.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	call pcc.cmd -c lib\font.c -o tmp\font.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\standard_font.c -o tmp\standard_font.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\memory.c -o tmp\memory.o -I "%cd%\include"
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
	call pcc.cmd -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	call p++.cmd -c C++\test.cpp -o tmp\test.o -I "%cd%\include"
echo ** 编译私有模块...
	copy /y private\* tmp\*.o >nul 2>nul
echo ** 链接目标文件...
	::Link
	ld -o tmp\kernel.o	-Ttext 0x11000^
	tmp\_start.o tmp\kernel_start.o tmp\main.o^
	tmp\shell.o tmp\font.o tmp\standard_font.o^
	tmp\hdd.o tmp\video.o tmp\mouse.o tmp\keyboard.o tmp\i8254.o tmp\i8254_asm.o tmp\dev_intr.o^
	tmp\i8259.o^
	tmp\task.o^
	tmp\memory.o tmp\kmalloc.o tmp\do_page_fault.o tmp\fat32.o^
	tmp\fun_asm.o tmp\fun_c.o tmp\trap.o tmp\trap_of_processor.o tmp\io.o^
	tmp\syscalls.o tmp\do_syscalls.o^
	tmp\test.o tmp\string.o tmp\GUI.o tmp\window.o tmp\init.o 2>>tmp\error.log
	echo.>>tmp\error.log
	echo objcopy's error:>>tmp\error.log
	objcopy -R .note -R .comment -S -O binary tmp\kernel.o tmp\kernel.bin 2>>tmp\error.log

	::Binary Copy
	copy /B tmp\loader.bin + tmp\kernel.bin KERNEL >nul 2>nul
	findstr No "tmp\error.log">nul&&goto Error||echo ** Compiled!
	del /f /s /q tmp\error.log >nul 2>nul
cls
	echo ** 编译成功完成！
title %t% - 正在写入内核
::Write kernel to Explorer.img
	echo ** 正在写入内核...
	taskkill /f /im virtualbox.exe >nul 2>nul
	WinImage ..\image\Explorer.img KERNEL /i /h /y


:run
::Start Virtual Machine
title %t% - 启动虚拟机...
	echo ** 正在启动虚拟机...
	taskkill /f /im virtualbox.exe>nul 2>nul
	VBoxManage.exe startvm "Ghost Bird 0.02"
	goto done

:about

cls
	echo %t% - %n%
	echo 我们的网站 www.GhostBirdOS.org
	echo --------------------------------------
	echo.
        if exist about (type about) else (echo Document not found.)
goto done
:log

cls
	echo %t% - %n%
	echo 我们的网站www.GhostBirdOS.org
	echo --------------------------------------
	echo.
	if exist "updates.txt" (notepad "updates.txt"&goto done)
goto done
:deltmp
cls
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo ** 临时文件清理完毕
goto done
:eof