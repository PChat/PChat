@ECHO OFF
REM A simple batch script to compile po files
SETLOCAL
CD /D %~dp0

TITLE Compiling translations...
ECHO Compiling translations...
ECHO. & ECHO.

FOR %%A IN (be ca cs de el en_GB es fi fr gl hi hu it ja ko lt mk nl
 pa pl pt ru sk sl sq sr sv th uk vi zh_CN zh_TW
) DO (
  IF NOT EXIST "..\win32\build\Win32\bin\locale\%%A\LC_MESSAGES" MD "..\win32\build\Win32\bin\locale\%%A\LC_MESSAGES"
  ECHO %%A
  "bin\msgfmt.exe" -cvo ..\win32\build\Win32\bin\locale\%%A\LC_MESSAGES\pchat.mo %%A.po
  ECHO _______________________________________________
)

:END
ECHO. & ECHO Finished!
ECHO.
ENDLOCAL
EXIT /B
