#pragma once
#include "../Display.hpp"


/**
 * @class JoyIt
 * @brief Display driver for HUB75 LED matrix hardware.
 * Translation layer wrapping the legacy panel_output driver.
 * @param int Horizontal size of the LED matrix in pixels.
 * @param int Vertical size of the LED matrix in pixels.
 */
class JoyIt : public Display {

    private:

        int _width;
        int _height;

    public:

        JoyIt(int width, int height);
        ~JoyIt();
        bool display(Image* img) override;

};
