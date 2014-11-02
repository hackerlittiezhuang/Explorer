@echo off
set t=APP Builder
color f0
title %t% - Welcome
echo Ghostbird ELF 应用编译器
echo.
echo 这是一个跨世纪的新改变。
echo 按任意键开始。
pause>nul
:main
title %t% - Compiling...
cls
	echo -^> 编译代码...
	echo ** 写入映像
	echo ** 启动虚拟机
echo.
echo ***********正在编译，请耐心等待......
gcc -c -fpermissive apps\first.c -o EXPLORER.ELF -I "%cd%\include"
if not %errorlevel%==0 (pause)
title %t% - Writing To Harddisk Image...
cls
	echo √ 编译代码
	echo -^> 写入映像...
	echo ** 启动虚拟机
echo.
echo ***********请在新弹出的窗口中单击“确定”
	taskkill /f /im virtualbox.exe>nul 2>nul
	taskkill /f /im winimage.exe>nul 2>nul
	WinImage ..\image\fat32.vhd explorer.elf /i /h /y
del /f /s /q explorer.elf
cls
::Start Virtual Machine
title %t% - 启动虚拟机...
	echo √ 编译代码
	echo √ 写入映像
	echo -^> 启动虚拟机...
echo.
echo ***********若长时间没反应或虚拟机启动黑屏请关闭后重新开始
	ping 127.1 /n 1 >nul 2>nul
	VBoxManage.exe startvm "Ghost Bird 0.02" >nul
cls
	echo √ 编译代码
	echo √ 写入映像
	echo √ 启动虚拟机
echo.
echo 按任意键重新编译
pause>nul
goto main