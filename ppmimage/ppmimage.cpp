/*
    This program generates a PPM image of the Italian flag.
    The flag consists of three vertical stripes: green, white, and red.
    The output is written to standard output in PPM (P3) format.
    (PS. Italy because mamma mia, why nottto?)
*/

#include <iostream>
#include <cmath>

int main() {
    int width = 600;   /* Width of the image */
    int height = 400;  /* Height of the image */

    /* Output the PPM header: format, width, height, and max color value */
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    /* Loop over each pixel row by row */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double red = 0.0, green = 0.0, blue = 0.0; /* Initialize color values */

            if (x < width / 3) {
                /* Green stripe (left third) - normalized RGB values */
                red = 0.0;
                green = 146.0 / 255.0;
                blue = 70.0 / 255.0;
            } else if (x < 2 * width / 3) {
                /* White stripe (middle third) */
                red = 1.0;
                green = 1.0;
                blue = 1.0;
            } else {
                /* Red stripe (right third) - normalized RGB values */
                red = 206.0 / 255.0;
                green = 43.0 / 255.0;
                blue = 55.0 / 255.0;
            }

            /* Convert normalized color values [0,1] to integer [0,255] */
            int ir = static_cast<int>(255.999 * red);
            int ig = static_cast<int>(255.999 * green);
            int ib = static_cast<int>(255.999 * blue);

            /* Output the pixel's RGB values */
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}