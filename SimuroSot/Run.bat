@echo off
TASKKILL /F /IM RobotSoccer.exe
TASKKILL /F /IM ConsoleDebugger.exe
CLS
set /p option= PRESS y TO RUN YOUR STRATEGY: 

IF "%option%" NEQ "y" EXIT
DEL C:\Strategy\blue\Team1.dll
COPY ..\SimuroSot\Release\Strategy.dll C:\Strategy\blue
REN C:\Strategy\blue\Strategy.dll Team1.dll
START /B ..\ConsoleDebugger.exe
IF EXIST C:\"Program Files (x86)"\"Robot Soccer v1.5a"\RobotSoccer.exe ( START C:\"Program Files (x86)"\"Robot Soccer v1.5a"\RobotSoccer.exe ) ELSE ( START C:\"Program Files"\"Robot Soccer v1.5a"\RobotSoccer.exe )
EXIT