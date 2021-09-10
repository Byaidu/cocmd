@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
cls
:loop
set thread=lock
for /l %%a in (0,1,255) do (
  set CAPI=API Call user32 GetAsyncKeyState ;%%a
  set KEY_%%a=!CAPI_Ret!
)
set thread=unlock
set CAPI=API Call kernel32 Sleep ;100
goto loop