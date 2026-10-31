// Use a Image rendered by a Game to display it on a screen
// Dispay is the mother class for all Drivers
// Even if the Image is bigger than the final screen resolution, the Driver should be able to scale it.
#pragma once

#include "Image.hpp"


/**
 * @class Display
 * @brief Abstract class for Drivers used to display Image on their corresponding device.
 */
class Display {

    public:

        virtual ~Display() = default;

        /**
         * @fn display
         * @brief Display the Image on the device managed by the driver.
         * @param Image* Image to display.
         */
        virtual bool display( Image* ) = 0;

};
