@echo off
cls
set b=0
:loop
echo #3.bat %b% 1>&2
set /a b+=1
ping -n 4 localhost >nul
goto loop