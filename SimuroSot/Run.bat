@echo off
TASKKILL /F /IM RobotSoccer.exe
TASKKILL /F /IM ConsoleDebugger.exe

DEL C:\Strategy\blue\Team1.dll
COPY ..\SimuroSot\Release\Strategy.dll C:\Strategy\blue
REN C:\Strategy\blue\Strategy.dll Team1.dll
EXIT