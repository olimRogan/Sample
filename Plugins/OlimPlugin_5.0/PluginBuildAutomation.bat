@echo off
:: 언리얼엔진 경로 (C, D)
SET ENGINE_DIR_C=C:\Program Files\Epic Games\UE_5.0\Engine
SET ENGINE_DIR_D=D:\Program Files\Epic Games\UE_5.0\Engine

:: 플러그인 경로
SET PROJECT_DIR=%~dp0

:: 플러그인 파일 찾기
FOR %%f IN (*.uplugin) DO FOR /F "tokens=1 delims=" %%i IN ("%%f") DO (
	SET filaname=%%i
)
:: 플러그인 파일명
SET PROJECT_FILE=%filaname%

SET RunUAT_DIR
:: 해당 경로에 RunUAT.bat 파일이 있는지 확인
if not exist %ENGINE_DIR_C% (
SET RunUAT_DIR=%ENGINE_DIR_D%\Build\BatchFiles\RunUAT.bat
) else (
SET RunUAT_DIR=%ENGINE_DIR_C%\Build\BatchFiles\RunUAT.bat
)

call "%RunUAT_DIR%" BuildPlugin -Rocket -Plugin="%PROJECT_DIR%%PROJECT_FILE%" -TargetPlatform=Win64 -package="%PROJECT_DIR%\Package"

