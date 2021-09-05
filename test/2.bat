@echo off
cls
set a=0
:loop
set thread=lock
set thread=unlock
echo #2.bat %a% 1>&2
set /a a+=1
ping -n 3 localhost >nul
goto loop