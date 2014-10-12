@echo off
::Public C Compiler
if "%debug%"=="y" goto main
goto quick
:error
cls
title %t% - Compile Failed
	color 0c
cls
	echo ** Sorry,compile failed.
	echo ** There is some error in the source code.
	echo.
	echo Here is error information:
	type tmp\error.log
pause>nul
"public make.cmd"
exit
:quick
title %t% - %2(Quick Mode)
gcc %1 %2 %3 %4 %5 %6 %7 %8 %9 2>nul
goto eof
:comps
title %t% - %2(Debug Mode)
gcc %1 %2 %3 %4 %5 %6 %7 %8 %9 2>>tmp\error.log
findstr rror "tmp\error.log">nul&&goto Error||echo ^$ %2 - Compiled
goto eof
:main
if exist %2 (goto comps) else (echo !! %2 is private module.&goto eof)
:eof