@echo off
set t=APP Builder
color f0
title %t% - Welcome
echo Ghostbird ELF Ӧ�ñ�����
echo.
echo ����һ�������͵��¸ı䡣
echo ���������ʼ��
pause>nul
:main
title %t% - Compiling...
cls
	echo -^> �������...
	echo ** д��ӳ��
	echo ** ���������
echo.
echo ***********���ڱ��룬�����ĵȴ�......
gcc -c -fpermissive apps\first.c -o EXPLORER.ELF -I "%cd%\include"
if not %errorlevel%==0 (pause)
title %t% - Writing To Harddisk Image...
cls
	echo �� �������
	echo -^> д��ӳ��...
	echo ** ���������
echo.
echo ***********�����µ����Ĵ����е�����ȷ����
	taskkill /f /im virtualbox.exe>nul 2>nul
	taskkill /f /im winimage.exe>nul 2>nul
	WinImage ..\image\fat32.vhd explorer.elf /i /h /y
del /f /s /q explorer.elf
cls
::Start Virtual Machine
title %t% - ���������...
	echo �� �������
	echo �� д��ӳ��
	echo -^> ���������...
echo.
echo ***********����ʱ��û��Ӧ�����������������رպ����¿�ʼ
	ping 127.1 /n 1 >nul 2>nul
	VBoxManage.exe startvm "Ghost Bird 0.02" >nul
cls
	echo �� �������
	echo �� д��ӳ��
	echo �� ���������
echo.
echo ����������±���
pause>nul
goto main