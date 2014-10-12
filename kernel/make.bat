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
	echo ^[请按任意键返回^]
pause > nul
goto main

:error
cls
title %t% 链接失败
	color 0c
cls
	echo 抱歉，编译失败。
	echo 源代码有一些错误。
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
title %t% 编译系统
if "%new13%"=="y" (title %t% - New13 Deceted)
	echo 版权所有 2013-2014 鬼鸟操作系统开发者。
	echo Copyright 2013-2014 by Ghost Bird OS Developer.
	echo 我们的网站:www.GhostBirdOS.org
	echo 功能号	备注
	echo.
	echo Enter	编译、装载并运行（快速模式）
	echo 1	仅运行
	echo 2	启动Virtual Box虚拟机
	echo 3	删除临时文件
	echo 4	检查并提交
	echo 5	仅检查
	echo 6	强制提交（危险）
	echo 7	安装向导
	echo 8	更新日志
	echo 9	关于Lab Explorer make system
	echo 0	Exit
	echo.
set /p command="请选择功能号："
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
echo 未知命令，按任意键返回。
pause > nul
goto main
:all
goto compile
:debug
set debug=y
goto compile

:Compile
cls
title %t% - 编译中...
echo 错误信息：>tmp\error.log
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
echo 拷贝私有模块...
	copy /y private\* tmp\*.o >nul 2>nul
echo 链接中...
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
	echo 由objcopy导致的错误：>>tmp\error.log
	objcopy -R .note -R .comment -S -O binary tmp\kernel.o tmp\kernel.bin 2>>tmp\error.log

	::Binary Copy
	copy /B tmp\loader.bin + tmp\kernel.bin KERNEL >nul 2>nul
	findstr No "tmp\error.log">nul&&goto Error||echo ** Compiled!
	del /f /s /q tmp\error.log >nul 2>nul
cls
	echo 内核已经编译完成。
title %t% 装载内核中.
::Write kernel to Explorer.img
	echo 使用WinImage向Explorer.img装载KERNEL中.
	taskkill /f /im virtualbox.exe >nul 2>nul
	WinImage ..\image\Explorer.img KERNEL /i /h /y


:run
::Start Virtual Machine
title %t% - 启动Virtual Box虚拟机
	echo 启动Virtual Box虚拟机.
	taskkill /f /im virtualbox.exe>nul 2>nul
	VBoxManage.exe startvm "Ghost Bird 0.02"
	goto done

:about

cls
	echo %t%编译系统
	echo 想获得更多信息，请访问www.GhostBirdOS.org
	echo.
        if exist about (type about) else (echo Document not found.)
goto done
:log

cls
	echo 正在打开%t%的更新日志
	echo 想获得更多信息，请访问www.GhostBirdOS.org
	echo.
	if exist "updates.txt" (notepad "updates.txt"&goto done)
	if exist "Smilodon Log.txt" (notepad "Smilodon Log.txt") else (echo Document not found.)
goto done
:deltmp
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo 临时文件已经被删除。
	goto done

:svn
if not exist init.cmd (echo 未安装！&goto setup)
call init.cmd
svn>nul 2>nul
if "%errorlevel%"=="9009" (echo Subversion 还没有被安装。&pause>nul&exit)
if "%1"=="coci" (cls&goto Checkout_Commit)
echo 安装完成，请重新执行之前的命令。
goto done
:setup
color b
title %t% 安装向导
echo 欢迎使用Svn Shell！
echo Svn Shell被Rose创造出来，被使用在GhostBird OS。
echo.
echo 如果你准备好了，就开始吧！
echo.
set /p usn="你的名字："
echo @echo off>init.cmd
echo set usn=%usn%>>init.cmd
cls
goto svn
:::::::Main

:co
if not exist init.cmd (goto svn)
title %t% 仅检查
echo 检查开始
svn co https://github.com/roselyu/Explorer
echo 检查完成
goto done

:ci
if not exist init.cmd (goto svn)
title %t% 强制提交
color 4
echo 危险！
echo [按任意键继续]
pause>nul
set /p msg="输入这个版本的描述："
echo 开始提交.
svn ci -m "%msg%" explorer --username %usn%
echo 完成！
goto done

:Checkout_Commit
if not exist init.cmd (goto svn)
title %t% 检查并提交
set /p msg="输入这个版本的描述："
echo 检查中.
svn co https://github.com/roselyu/Explorer
echo 检查中.
svn ci -m "%msg%" explorer --username %usn%
echo 完成！
goto done

:eof