@echo off
chcp 65001 >nul 2>&1
setlocal EnableDelayedExpansion

:: ============================================================
::  Dobby Project - 编译 & 安装 & 运行 脚本
:: ============================================================

:: --- 配置区 ---
set "JAVA_HOME=C:\Program Files\Android\Android Studio\jbr"
set "PATH=%JAVA_HOME%\bin;%PATH%"
set "ADB=C:\Users\user\AppData\Local\Android\Sdk\platform-tools\adb.exe"
set "ADB_DEVICE=-s 37171FDJH001TH"
set "PACKAGE=com.example.dobbyproject"
set "ACTIVITY=%PACKAGE%/.MainActivity"
set "APK_PATH=app\build\outputs\apk\debug\app-debug.apk"

:: --- 解析参数 ---
if "%~1"=="" goto :build_install_run
if /i "%~1"=="build"   goto :build_only
if /i "%~1"=="install"  goto :install_only
if /i "%~1"=="run"      goto :run_only
if /i "%~1"=="clean"    goto :clean
if /i "%~1"=="release"  goto :build_release
if /i "%~1"=="help"     goto :help
echo [错误] 未知参数: %~1
goto :help

:: ============================================================
:help
echo.
echo 用法: build_and_run.bat [命令]
echo.
echo 命令:
echo   (无参数)   编译 + 安装 + 启动 (默认)
echo   build      仅编译 Debug APK
echo   install    仅安装到设备
echo   run        仅启动应用
echo   release    编译 Release APK
echo   clean      清理构建产物
echo   help       显示此帮助
echo.
goto :eof

:: ============================================================
:clean
echo [1/1] 清理构建产物...
call gradlew.bat clean
if %errorlevel% neq 0 (echo [错误] 清理失败 & exit /b 1)
echo [完成] 清理成功
goto :eof

:: ============================================================
:build_only
echo [1/1] 编译 Debug APK...
call gradlew.bat assembleDebug
if %errorlevel% neq 0 (echo [错误] 编译失败 & exit /b 1)
echo [完成] APK 位于: %APK_PATH%
goto :eof

:: ============================================================
:build_release
echo [1/1] 编译 Release APK...
call gradlew.bat assembleRelease
if %errorlevel% neq 0 (echo [错误] 编译失败 & exit /b 1)
echo [完成] Release APK 位于: app\build\outputs\apk\release\
goto :eof

:: ============================================================
:install_only
echo [1/1] 安装到设备...
"%ADB%" %ADB_DEVICE% install -r "%APK_PATH%"
if errorlevel 1 (
    echo [提示] 签名不匹配, 尝试卸载后重新安装...
    "%ADB%" %ADB_DEVICE% uninstall %PACKAGE%
    "%ADB%" %ADB_DEVICE% install "%APK_PATH%"
    if !errorlevel! neq 0 (echo [错误] 安装失败 & exit /b 1)
)
echo [完成] 安装成功
goto :eof

:: ============================================================
:run_only
echo [1/1] 启动应用...
"%ADB%" %ADB_DEVICE% shell am start -n %ACTIVITY%
if %errorlevel% neq 0 (echo [错误] 启动失败 & exit /b 1)
echo [完成] 应用已启动
goto :eof

:: ============================================================
:build_install_run
echo ====== 编译 + 安装 + 运行 ======
echo.

echo [1/3] 编译 Debug APK...
call gradlew.bat assembleDebug
if %errorlevel% neq 0 (echo [错误] 编译失败 & exit /b 1)
echo [1/3] 编译成功
echo.

echo [2/3] 安装到设备...
"%ADB%" %ADB_DEVICE% install -r "%APK_PATH%"
if errorlevel 1 (
    echo [提示] 签名不匹配, 尝试卸载后重新安装...
    "%ADB%" %ADB_DEVICE% uninstall %PACKAGE%
    "%ADB%" %ADB_DEVICE% install "%APK_PATH%"
    if !errorlevel! neq 0 (echo [错误] 安装失败 & exit /b 1)
)
echo [2/3] 安装成功
echo.

echo [3/3] 启动应用...
"%ADB%" %ADB_DEVICE% shell am start -n %ACTIVITY%
if %errorlevel% neq 0 (echo [错误] 启动失败 & exit /b 1)
echo [3/3] 应用已启动
echo.

echo ====== 全部完成 ======
goto :eof
