@echo off
::::::::::::::::::::::::::::::::::::::
::  Public Make - Powered by Smilodon
::  Work with Explorer v14
::
::    WRITTEN BY ROSE<roselyu@163.com>
::::::::::::::::::::::::::::::::::::::
if not exist tmp (md tmp)
if not exist private (md private)
set t=Public Make 0.2
set n=For v14 Series
if exist new13 (set new13=y)
goto main
:done
color 0f
title %t% - Done
	echo.
	echo.
	echo.
	echo ^[Press any key to return^]
pause > nul
goto main

:error
cls
title %t% - Link Failed
	color 0c
cls
	echo ** Sorry,compile failed.
	echo ** There is some error in the source code.
	echo.
	echo Here is error information:
	type tmp\error.log
goto done

::Main
:main
color 0f
cls
set command=
set debug=
title %t% - %n%
        echo Our website:www.GhostBirdOS.org
	echo ==============================================================
	echo.
	echo + Enter - Make and Launch (Debug Mode)
	echo +     1 - Make and Launch (Fast Mode)
	echo.
	echo +     2 - Run Virtual Machine
	echo +     3 - Delete Temporary Files
	echo +     4 - Publish this build
	ECHO.
	echo +     5 - About ^& Help
	echo +     6 - Update Log
	echo +     7 - Go to Lab Explorer Make System
	echo.
	echo +     0 - Exit
	echo.
	echo ------------[   PRESS ENTER to Compile,Load,Run   ]-----------
	echo.
set /p command="Function:"
if "%command%"=="" goto all
if "%command%"=="1" goto debug
if "%command%"=="2" goto run
if "%command%"=="3" goto deltmp
if "%command%"=="4" goto publish
if "%command%"=="5" goto about
if "%command%"=="6" goto log
if "%command%"=="7" (start make.bat&exit)
if "%command%"=="0" exit
cls
echo ** Unknown Command. Press Any Key to Return.
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
echo Are you sure to PUBLISH?(Press any key for 3 times)
pause>nul
cls
echo Are you sure to PUBLISH?(Press any key for 2 times)
pause>nul
cls
echo Are you sure to PUBLISH?(Press any key to continue)
color f
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo ** Temporary files have been deleted!
cd ..\..\..\
svnshell.cmd coci
exit
:Compile
cls
title %t% - Compiling...
echo ==============================================================>tmp\error.log
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
	nasm -f elf process\kernel_thread.asm -o tmp\Creat_kernel_thread.o
	nasm -f elf process\do_fork.asm -o tmp\do_fork.o
	nasm -f elf process\init_exe_environment.asm -o tmp\init_exe_environment.o
	nasm -f elf process\Creat_process.asm -o tmp\Creat_process.o
	call pcc.cmd -c arch\x86\kernel\kernel_start.c -o tmp\kernel_start.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\i8254.c -o tmp\i8254.o -I "%cd%\include"
	call pcc.cmd -c init\main.c -o tmp\main.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\font.c -o tmp\font.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\memory.c -o tmp\memory.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\kmalloc.c -o tmp\kmalloc.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\do_page_fault.c -o tmp\do_page_fault.o -I "%cd%\include"
	call pcc.cmd -c process\process.c -o tmp\process.o -I "%cd%\include"
	call pcc.cmd -c process\exec.c -o tmp\exec.o -I "%cd%\include"
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
echo ** Copying Private Module......
	copy /y private\* tmp\*.o >nul 2>nul
echo ** Linking...
	::Link
	ld -o tmp\kernel.o	-Ttext 0x11000^
	tmp\_start.o tmp\kernel_start.o tmp\main.o^
	tmp\shell.o tmp\font.o^
	tmp\hdd.o tmp\video.o tmp\mouse.o tmp\keyboard.o tmp\i8254.o tmp\i8254_asm.o tmp\dev_intr.o^
	tmp\i8259.o^
	tmp\memory.o tmp\kmalloc.o tmp\do_page_fault.o tmp\fat32.o^
	tmp\process.o tmp\Creat_kernel_thread.o tmp\Creat_process.o tmp\do_fork.o tmp\init_exe_environment.o tmp\exec.o^
	tmp\fun_asm.o tmp\fun_c.o tmp\trap.o tmp\trap_of_processor.o tmp\io.o^
	tmp\syscalls.o tmp\do_syscalls.o^
	tmp\test.o tmp\string.o tmp\GUI.o tmp\window.o tmp\init.o 2>>tmp\error.log
	echo.>>tmp\error.log
	echo =====and these errors caused objcopy 's error:>>tmp\error.log
	objcopy -R .note -R .comment -S -O binary tmp\kernel.o tmp\kernel.bin 2>>tmp\error.log

	::Binary Copy
	copy /B tmp\loader.bin + tmp\kernel.bin KERNEL >nul 2>nul
	findstr No "tmp\error.log">nul&&goto Error||echo ** Compiled!
	del /f /s /q tmp\error.log >nul 2>nul
cls
	echo ** Kernel has been Compiled!
title %t% - Writing Kernel...
::Write kernel to Explorer.img
	echo ** Writing Kernel......
	taskkill /f /im virtualbox.exe >nul 2>nul
	WinImage ..\image\Explorer.img KERNEL /i /h /y


:run
::Start Virtual Machine
title %t% - Starting Virtual Machine......
	echo ** Starting Virtual Machine......
	taskkill /f /im virtualbox.exe>nul 2>nul
	VBoxManage.exe startvm "Ghost Bird 0.02"
	goto done

:about

cls
	echo %t% - %n%
	echo For more information,please visit :www.GhostBirdOS.org
	echo ==============================================================
	echo.
        if exist about (type about) else (echo Document not found.)
goto done
:log

cls
	echo %t% - %n%
	echo For more information,please visit :www.GhostBirdOS.org
	echo ==============================================================
	echo.
	if exist "updates.txt" (notepad "updates.txt"&goto done)
	if exist "Smilodon Log.txt" (notepad "Smilodon Log.txt") else (echo Document not found.)
goto done
:deltmp
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo ** Temporary files have been deleted!
goto done
:eof