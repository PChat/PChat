@echo off
set PATH=%PATH%;%PROGRAMW6432%\7-Zip;%PROGRAMFILES6432%\TortoiseHg
hg clone https://bitbucket.org/ZachThibeau/pchat-dev pchat
rmdir /q /s pchat\.hg
7z a pchat-head.7z pchat
rmdir /q /s pchat
pause
