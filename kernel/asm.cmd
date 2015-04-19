@echo off
::Powerful C Compiler
goto comps
:error
cls
title %t% - Error Compiling %2
	color 0c
cls
	echo ** Error Compiling %2:
	type tmp\error.log
	echo [Press any key to return]
pause>nul
start make.cmd %comct%
exit

:comps
title %t% - Compiling %2...
nasm %1 %2 %3 %4 %5 %6 %7 %8 %9 2>>tmp\error.log
findstr rror "tmp\error.log">nul&&goto error||echo ^$ Compile %2 - Done
goto eof
:eof