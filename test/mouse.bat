@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
cls
set thread=lock
set CAPI=Mem Alloc 8
set lpPoint=!CAPI_Ret!
set thread=unlock
:loop
set thread=lock
set CAPI=API Call user32 GetCursorPos ;!lpPoint!
set CAPI=Mem Print ;!lpPoint! ;x ;y
set MOUSE_X=!x!
set MOUSE_Y=!y!
set thread=unlock
set CAPI=API Call kernel32 Sleep ;100
goto loop