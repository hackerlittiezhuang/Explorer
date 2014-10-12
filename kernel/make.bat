::  Lab Explorer make system
::  Powered by Smilodon
::    WRITTEN BY ROSE<roselyu@163.com>Hu wenjie(CN)<1@GhostBirdOS.org>
@echo off
if not exist tmp (md tmp)
if not exist private (md private)
set t=Lab Explorer
if exist new13 (set new13=y)
goto main
:done
color 0f
title %t%
	echo.
	echo.
	echo.
	echo ^[�밴���������^]
pause > nul
goto main

:error
cls
title %t% ����ʧ��
	color 0c
cls
	echo ��Ǹ������ʧ�ܡ�
	echo Դ������һЩ����
	echo.
	type tmp\error.log
goto done

::Main
:main
color 0f
if "%new13%"=="y" (color 0e)
cls
set command=
set debug=
title %t% ����ϵͳ
if "%new13%"=="y" (title %t% - New13 Deceted)
	echo ��Ȩ���� 2013-2014 �������ϵͳ�����ߡ�
	echo Copyright 2013-2014 by Ghost Bird OS Developer.
	echo ���ǵ���վ:www.GhostBirdOS.org
	echo ���ܺ�	��ע
	echo.
	echo Enter	���롢װ�ز����У�����ģʽ��
	echo 1	������
	echo 2	����Virtual Box�����
	echo 3	ɾ����ʱ�ļ�
	echo 4	��鲢�ύ
	echo 5	�����
	echo 6	ǿ���ύ��Σ�գ�
	echo 7	��װ��
	echo 8	������־
	echo 9	����Lab Explorer make system
	echo 0	Exit
	echo.
set /p command="��ѡ���ܺţ�"
if "%command%"=="" goto all
if "%command%"=="1" goto debug
if "%command%"=="2" goto run
if "%command%"=="3" goto deltmp
if "%command%"=="4" goto Checkout_Commit
if "%command%"=="5" goto co
if "%command%"=="6" goto ci
if "%command%"=="7" goto setup
if "%command%"=="8" goto log
if "%command%"=="9" goto about
if "%command%"=="0" exit
cls
echo δ֪�������������ء�
pause > nul
goto main
:all
goto compile
:debug
set debug=y
goto compile

:Compile
cls
title %t% - ������...
echo ������Ϣ��>tmp\error.log
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
	call pcc.cmd -c arch\x86\kernel\i8254.c -o tmp\i8254.o -I "%cd%\include"
	call pcc.cmd -c init\main.c -o tmp\main.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\font.c -o tmp\font.o -I "%cd%\include"
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
	call pcc.cmd -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	call p++.cmd -c C++\test.cpp -o tmp\test.o -I "%cd%\include"
echo ����˽��ģ��...
	copy /y private\* tmp\*.o >nul 2>nul
echo ������...
	::Link
	ld -o tmp\kernel.o	-Ttext 0x11000^
	tmp\_start.o tmp\kernel_start.o tmp\main.o^
	tmp\shell.o tmp\font.o^
	tmp\hdd.o tmp\video.o tmp\mouse.o tmp\keyboard.o tmp\i8254.o tmp\i8254_asm.o tmp\dev_intr.o^
	tmp\i8259.o^
	tmp\task.o^
	tmp\memory.o tmp\kmalloc.o tmp\do_page_fault.o tmp\fat32.o^
	tmp\fun_asm.o tmp\fun_c.o tmp\trap.o tmp\trap_of_processor.o tmp\io.o^
	tmp\syscalls.o tmp\do_syscalls.o^
	tmp\test.o tmp\string.o tmp\GUI.o tmp\window.o tmp\init.o 2>>tmp\error.log
	echo.>>tmp\error.log
	echo ��objcopy���µĴ���>>tmp\error.log
	objcopy -R .note -R .comment -S -O binary tmp\kernel.o tmp\kernel.bin 2>>tmp\error.log

	::Binary Copy
	copy /B tmp\loader.bin + tmp\kernel.bin KERNEL >nul 2>nul
	findstr No "tmp\error.log">nul&&goto Error||echo ** Compiled!
	del /f /s /q tmp\error.log >nul 2>nul
cls
	echo �ں��Ѿ�������ɡ�
title %t% װ���ں���.
::Write kernel to Explorer.img
	echo ʹ��WinImage��Explorer.imgװ��KERNEL��.
	taskkill /f /im virtualbox.exe >nul 2>nul
	WinImage ..\image\Explorer.img KERNEL /i /h /y


:run
::Start Virtual Machine
title %t% - ����Virtual Box�����
	echo ����Virtual Box�����.
	taskkill /f /im virtualbox.exe>nul 2>nul
	VBoxManage.exe startvm "Ghost Bird 0.02"
	goto done

:about

cls
	echo %t%����ϵͳ
	echo ���ø�����Ϣ�������www.GhostBirdOS.org
	echo.
        if exist about (type about) else (echo Document not found.)
goto done
:log

cls
	echo ���ڴ�%t%�ĸ�����־
	echo ���ø�����Ϣ�������www.GhostBirdOS.org
	echo.
	if exist "updates.txt" (notepad "updates.txt"&goto done)
	if exist "Smilodon Log.txt" (notepad "Smilodon Log.txt") else (echo Document not found.)
goto done
:deltmp
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo ��ʱ�ļ��Ѿ���ɾ����
	goto done

:svn
if not exist init.cmd (echo δ��װ��&goto setup)
call init.cmd
svn>nul 2>nul
if "%errorlevel%"=="9009" (echo Subversion ��û�б���װ��&pause>nul&exit)
if "%1"=="coci" (cls&goto Checkout_Commit)
echo ��װ��ɣ�������ִ��֮ǰ�����
goto done
:setup
color b
title %t% ��װ��
echo ��ӭʹ��Svn Shell��
echo Svn Shell��Rose�����������ʹ����GhostBird OS��
echo.
echo �����׼�����ˣ��Ϳ�ʼ�ɣ�
echo.
set /p usn="������֣�"
echo @echo off>init.cmd
echo set usn=%usn%>>init.cmd
cls
goto svn
:::::::Main

:co
if not exist init.cmd (goto svn)
title %t% �����
echo ��鿪ʼ
svn co https://github.com/roselyu/Explorer
echo ������
goto done

:ci
if not exist init.cmd (goto svn)
title %t% ǿ���ύ
color 4
echo Σ�գ�
echo [�����������]
pause>nul
set /p msg="��������汾��������"
echo ��ʼ�ύ.
svn ci -m "%msg%" explorer --username %usn%
echo ��ɣ�
goto done

:Checkout_Commit
if not exist init.cmd (goto svn)
title %t% ��鲢�ύ
set /p msg="��������汾��������"
echo �����.
svn co https://github.com/roselyu/Explorer
echo �����.
svn ci -m "%msg%" explorer --username %usn%
echo ��ɣ�
goto done

:eof