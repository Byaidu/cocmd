@echo off
cls
set a=0
set b=0
:loop
rem echo #1.bat %time% 1>&2
title time:%time%(1.bat) a:%a%(2.bat) b:%b%(3.bat)
if %a% equ 3 (
  echo #1.bat lock 1>&2
  set thread=lock
  ping -n 10 localhost >nul
  set /a a+=1
  set thread=unlock
  echo #1.bat unlock 1>&2
)
ping -n 2 localhost >nul
goto loop