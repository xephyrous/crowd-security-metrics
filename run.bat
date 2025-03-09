@echo off
setlocal enabledelayedexpansion

:: Command line args
set BUILD_TYPE=%1
set TARGET_TYPE=%2

:: Default values
if "%BUILD_TYPE%"=="" set BUILD_TYPE=debug
if "%TARGET_TYPE%"=="" set TARGET_TYPE=main

if "%BUILD_TYPE%"=="debug" (
    set OUTPUT_DIR=%BUILD_DIR%\Debug
) else if "%BUILD_TYPE%"=="release" (
    set OUTPUT_DIR=%BUILD_DIR%\Release
)

:: Set build directory
set BUILD_DIR=build
set ASSETS_SRC=%CD%\Assets
set ASSETS_DEST=%CD%\%OUTPUT_DIR%\Assets

:: Check if build directory exists
if not exist %BUILD_DIR% (
    echo Build directory does not exist. Running full CMake configuration...
    mkdir %BUILD_DIR%
    cd %BUILD_DIR%
    cmake ..
    cmake --build . --config Debug

    :: Ensure Debug directory exists before copying assets
    xcopy /E /I /Y "%ASSETS_SRC%" "%ASSETS_DEST%"

    cd Debug
    start CrowdSecurityMetrics.exe
    start FrameServer.exe
    exit /b
)

:: Check if CMakeLists.txt was modified since the last build
for %%F in (CMakeLists.txt) do set CMAKE_TIME=%%~tF
for %%F in (%BUILD_DIR%\CMakeCache.txt) do set BUILD_TIME=%%~tF

if "%CMAKE_TIME%" GTR "%BUILD_TIME%" (
    echo CMakeLists.txt changed. Running full reconfiguration...
    rmdir /s /q %BUILD_DIR%
    mkdir %BUILD_DIR%
    cd %BUILD_DIR%
    cmake ..
    cmake --build . --config Debug

    :: Ensure Debug directory exists before copying assets
    if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%
    xcopy /E /I /Y "%ASSETS_SRC%" "%ASSETS_DEST%"

    cd Debug
    start CrowdSecurityMetrics.exe
    start FrameServer.exe
    exit /b
)

:: If only source files changed, just rebuild
echo Running incremental build...
cd %BUILD_DIR%
cmake --build . --config Debug

:: Ensure Assets folder is in Debug directory
if not exist "%ASSETS_DEST%" (
    echo Copying assets...
    xcopy /E /I /Y "%ASSETS_SRC%" "%ASSETS_DEST%"
)

cd Debug
start CrowdSecurityMetrics.exe
start FrameServer.exe