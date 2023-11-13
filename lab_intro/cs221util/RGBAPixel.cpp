#include "RGBAPixel.h"

namespace cs221util {
    RGBAPixel::RGBAPixel() {
        r = 255;
        g = 255;
        b = 255;
        a = 1.0;
    }

    RGBAPixel::RGBAPixel(unsigned int red, unsigned int green, unsigned int blue) {
        r = red;
        g = green;
        b = blue;
        a = 1.0;
    }

    RGBAPixel::RGBAPixel(unsigned int red, unsigned int green, unsigned int blue, double alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }
}

