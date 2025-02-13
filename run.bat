@echo off
setlocal enabledelayedexpansion

:: Set build directory and executable name
set BUILD_DIR=build
set DEBUG_DIR=%BUILD_DIR%\Debug
set EXE_NAME=CrowdSecurityMetrics.exe
set ASSETS_SRC=%CD%\Assets
set ASSETS_DEST=%CD%\%DEBUG_DIR%\Assets

:: Check if build directory exists
if not exist %BUILD_DIR% (
    echo Build directory does not exist. Running full CMake configuration...
    mkdir %BUILD_DIR%
    cd %BUILD_DIR%
    cmake ..
    cmake --build . --config Debug

    :: Ensure Debug directory exists before copying assets
    if not exist %DEBUG_DIR% mkdir %DEBUG_DIR%
    xcopy /E /I /Y "%ASSETS_SRC%" "%ASSETS_DEST%"

    cd Debug
    start %EXE_NAME%
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
    if not exist %DEBUG_DIR% mkdir %DEBUG_DIR%
    xcopy /E /I /Y "%ASSETS_SRC%" "%ASSETS_DEST%"

    cd Debug
    start %EXE_NAME%
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
start %EXE_NAME%