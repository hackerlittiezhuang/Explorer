@echo off
::Public C++ Compiler
if "%debug%"=="y" goto main
goto quick
:error
cls
title %t% - ����ʧ��
	color 0c
cls
	echo ** ��Ǹ �������
	echo.
	echo ��������:
	type tmp\error.log|more
	echo [�����������]
pause>nul
start make.bat
exit
:quick
title %t% - %2(����)
g++ %1 %2 %3 %4 %5 %6 %7 %8 %9 2>nul
goto eof
:comps
title %t% - %2(����)
g++ %1 %2 %3 %4 %5 %6 %7 %8 %9 2>>tmp\error.log
findstr rror "tmp\error.log">nul&&goto error||echo ^$ %2 - ������
goto eof
:main
if exist %2 (goto comps) else (echo !! %2 ��˽��ģ��.&goto eof)
:eof
