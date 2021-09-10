@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
cls
set MOUSE_X=0
set MOUSE_Y=0
for /l %%a in (0,1,255) do set KEY_%%a=0
:loop
set all=
for /l %%a in (0,1,255) do (
  if not !KEY_%%a!==0 (
    set all=%%a,!all!
  )
)
cls
echo Time: !time!
echo Mouse: !MOUSE_X!,!MOUSE_Y!
echo Key: !all!
set CAPI=API Call kernel32 Sleep ;100
goto loop