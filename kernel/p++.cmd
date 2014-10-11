@echo off
::Public C Compiler

goto main
:comps
title %t% - %2
g++ %1 %2 %3 %4 %5 %6 %7 %8 %9 2>>tmp\error.log
goto eof
:main
if exist %2 (goto comps) else (echo ** %2 is private module.)
:eof