@echo off
chcp 65001 >nul 2>&1
setlocal EnableDelayedExpansion

if "%~1"=="" (
    echo 用法: logcat.bat 过滤词1 [过滤词2] [过滤词3] ...
    echo 示例:
    echo   logcat.bat Patch
    echo   logcat.bat Patch MiniMap
    echo   logcat.bat Patch MiniMap RADAR DumpStr
    echo   logcat.bat SFK          (查看所有项目日志^)
    exit /b 1
)

set "ADB=C:\Users\user\AppData\Local\Android\Sdk\platform-tools\adb.exe"
set "ADB_DEVICE=-s 37171FDJH001TH"

set "ARGS="
:loop
if "%~1"=="" goto run
set "ARGS=!ARGS! /C:"%~1""
shift
goto loop

:run
echo [logcat] 设备: %ADB_DEVICE%
echo [logcat] 过滤: %ARGS%
echo [logcat] Ctrl+C 停止
echo ─────────────────────────────────
%ADB% %ADB_DEVICE% logcat -c
%ADB% %ADB_DEVICE% logcat | findstr %ARGS%
