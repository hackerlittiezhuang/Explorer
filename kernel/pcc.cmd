@echo off
::Powerful C Compiler
if "%debug%"=="y" goto main
goto quick
:error
cls
title %t% - Powerful C Compiler
	color 0c
cls
	echo ** 抱歉 编译出错
	echo.
	echo 错误详情:（按任意键向下）
	type tmp\error.log|more
	echo [按任意键返回]
pause>nul
start make.bat %comct%
exit
:quick
title %t% - %2(快速)
gcc %1 %2 %3 %4 %5 %6 %7 %8 %9 2>nul
goto eof
:comps
title %t% - %2(调试)
gcc %1 %2 %3 %4 %5 %6 %7 %8 %9 2>>tmp\error.log
findstr rror "tmp\error.log">nul&&goto error||echo ^$ %2 - 编译了
goto eof
:main
if exist %2 (goto comps) else (echo !! %2 是私有模块.&goto eof)
:eof
