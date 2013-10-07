@echo off
TASKKILL /F /IM RobotSoccer.exe
TASKKILL /F /IM Debugger.exe

DEL C:\Strategy\blue\Team1.dll
COPY ..\SimuroSot\Release\Strategy.dll C:\Strategy\blue
REN C:\Strategy\blue\Strategy.dll Team1.dll

DEL C:\Strategy\yellow\Team2.dll
COPY ..\SimuroSot\Release\Strategy.dll C:\Strategy\yellow
REN C:\Strategy\yellow\Strategy.dll Team2.dll

EXIT
