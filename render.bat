@echo off
g++ main.cpp -o main.exe
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b
)
main.exe > image.ppm
magick image.ppm image.png
start image.png