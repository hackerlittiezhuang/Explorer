@echo off
::::::::::::::::::::::::::::::::::::::
::  ������ʽ - Smilodon ����
::  �������İ�
::  ������ Explorer v14
::
::    ROSE��д Rose<roselyu@163.com>
::::::::::::::::::::::::::::::::::::::
chcp 936
::�л����������ı���ҳ

if not exist tmp (md tmp)
if not exist private (md private)
set t=������ʽ 0.2 ���İ�
set n=v14ϵ������
goto main
:done
<<<<<<< HEAD:kernel/Make.cmd
title %t% - ���
=======
title %t% - Done
>>>>>>> explorer/master:kernel/Public Make.cmd
	echo.
	echo.
	echo.
	echo ^[�����������^]
pause > nul
goto main

:error
cls
title %t% - �������
	color 0c
cls
	echo ** ��Ǹ �������
	echo ** Դ�����п��ܳ���һЩ����
	echo.
	echo ��������:
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
    	echo ���ǵ���վ www.GhostBirdOS.org
	echo --------------------------------------
	echo.
	echo +    Enter - ���������������(����)
	echo +        1 - ���������������(����)
	echo.
	echo +        2 - ���������
	echo +        3 - ɾ����ʱ�ļ�
	echo +        4 - ����
	ECHO.
	echo +        5 - ���ںͰ���
	echo +        6 - �鿴������־
	echo.
	echo +        0 - �˳�
	echo.
	echo --------------------------------------
	echo [   ���� ENTER ִ�й��������������  ]
	echo.
set /p command="����:"
if "%command%"=="" goto all
if "%command%"=="1" goto debug
if "%command%"=="2" goto run
if "%command%"=="3" goto deltmp
if "%command%"=="4" goto publish
if "%command%"=="5" goto about
if "%command%"=="6" goto log
if "%command%"=="0" exit
echo ** δ֪���� ���������
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
echo ��ȷ��Ҫ������(���������)
=======
echo Are you sure to PUBLISH?(Press any key to continue)
>>>>>>> explorer/master:kernel/Public Make.cmd
color f
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo ** ��ʱ�ļ��������
cd ..\..\..\
svnshell.cmd coci
exit
:Compile
cls
mode con cols=80 lines=25
title %t% - ���ڱ���...
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
echo ** ����˽��ģ��...
	copy /y private\* tmp\*.o >nul 2>nul
echo ** ����Ŀ���ļ�...
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
	echo ** ����ɹ���ɣ�
title %t% - ����д���ں�
::Write kernel to Explorer.img
	echo ** ����д���ں�...
	taskkill /f /im virtualbox.exe >nul 2>nul
	WinImage ..\image\Explorer.img KERNEL /i /h /y


:run
::Start Virtual Machine
title %t% - ���������...
	echo ** �������������...
	taskkill /f /im virtualbox.exe>nul 2>nul
	VBoxManage.exe startvm "Ghost Bird 0.02"
	goto done

:about

cls
	echo %t% - %n%
	echo ���ǵ���վ www.GhostBirdOS.org
	echo --------------------------------------
	echo.
        if exist about (type about) else (echo Document not found.)
goto done
:log

cls
	echo %t% - %n%
	echo ���ǵ���վwww.GhostBirdOS.org
	echo --------------------------------------
	echo.
	if exist "updates.txt" (notepad "updates.txt"&goto done)
goto done
:deltmp
cls
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo ** ��ʱ�ļ��������
goto done
:eof