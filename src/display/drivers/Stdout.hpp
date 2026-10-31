#pragma once

#include "../Display.hpp"


/**
 * @class Stdout
 * @brief Display driver to display an Image to standard output.
 */
class Stdout: public Display {

    public:

        ~Stdout() = default;
        bool display( Image* ) override;

};
