#pragma once
#include <SDL2/SDL.h>

#include "../Display.hpp"


/**
 * @class Sdl
 * @brief Display driver to display an Image on a SDL-managed window.
 * @param int Horizontale size of the window in pixels.
 * @param int Vertical size of the window in pixels.
 */
class Sdl: public Display {

    private:

        int _size_x;
        int _size_y;
        SDL_Event _event;
        SDL_Renderer* _renderer;
        SDL_Window* _window;

    public:

        Sdl(int x, int y);
        ~Sdl();
        bool display(Image* ) override;

};
