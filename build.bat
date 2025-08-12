@echo off
setlocal

:: Set SDL3 base directory (update this path if needed)
set SDL3_DIR=C:\Users\ashmi\Downloads\SDL3-devel-3.2.16-mingw\SDL3-3.2.16\i686-w64-mingw32
set INCLUDE=%SDL3_DIR%\include
set LIB=%SDL3_DIR%\lib
set BIN=%SDL3_DIR%\bin

:: Compiler settings
set CC=g++
set SRC=selection.cpp
set OUT=sdl_app.exe

echo === Building SDL3 App ===
%CC% %SRC% -std=c++17 -I%INCLUDE% -L%LIB% -lSDL3 -o %OUT%

if %errorlevel% neq 0 (
    echo *** BUILD FAILED ***
    pause
    exit /b 1
)

echo.
echo === Copying SDL3.dll ===
copy /y "%BIN%\SDL3.dll" . >nul

echo.
echo *** BUILD SUCCEEDED ***
echo Run %OUT% to test your window.
pause
