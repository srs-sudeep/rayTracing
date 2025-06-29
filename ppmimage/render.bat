@echo off
g++ ppmimage.cpp -o ppmimage.exe
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b
)
ppmimage.exe > image.ppm
magick image.ppm image.png
start image.png