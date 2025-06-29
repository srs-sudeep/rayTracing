#!/bin/bash
# Compile the C++ source file
g++ ppmimage.cpp -o ppmimage
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Run the program and save output to image.ppm
./ppmimage > image.ppm

# Convert PPM to PNG (requires ImageMagick)
if command -v magick >/dev/null 2>&1; then
    magick image.ppm image.png
    xdg-open image.png 2>/dev/null &
else
    echo "ImageMagick not found. Skipping PNG conversion."
fi