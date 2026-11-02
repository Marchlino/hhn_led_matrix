#include "JoyIt.hpp"
#include "logger.h"

// Include the panel_output driver
#include "panel_output.h"


JoyIt::JoyIt(int width, int height) : _width(width), _height(height) {

    logger(0, "Initializing HUB75 display driver...");

    panel_output::initialize(width, height);

    logger(0, "HUB75 display driver initialized");

}


JoyIt::~JoyIt() {

    logger(0, "Shutting down HUB75 display driver...");
    panel_output::shutdown();

}


bool JoyIt::display(Image* img) {

    if (!img) {
        logger(2, "Attempted to display null Image");
        return false;
    }

    // Iterate through each pixel and send to hardware
    for (int y = 0; y < _height; y++) {

        for (int x = 0; x < _width; x++) {

            // Get pixel from image using view coordinates
            float vw = (float)x / _width;
            float vh = (float)y / _height;
            RGB pixel = img->getPixelInView(vw, vh);

            // Extract RGB channels (pixel format: 0xBBGGRR)
            uint8_t r = pixel & 0xFF;
            uint8_t g = (pixel >> 8) & 0xFF;
            uint8_t b = (pixel >> 16) & 0xFF;

            // Send to hardware
            panel_output::set_pixel(x, y, r, g, b);

        }

    }

    // Flush frame to display
    return panel_output::present();

}
