
#include "Sdl.hpp"
#include <cstdio>

#define R_MASK 0xff
#define G_MASK 0xff00
#define B_MASK 0xff0000

Sdl::Sdl(int x, int y) {

    this->size_x = x;
    this->size_y = y;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(x, y, 0, &window, &renderer);

}

Sdl::~Sdl() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

bool Sdl::display(Image* img) {

    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        return false;

    float steps_x = 1./this->size_x;
    float steps_y = 1./this->size_y;
    int x = 0;
    int y = 0;

    for (float l = 0.; l < 1.; l += steps_y) {

        for (float k = 0.; k < 1.; k += steps_x) {

            RGB p = img->getPixelInView(k, l);
            SDL_SetRenderDrawColor(renderer,  p & R_MASK, (p & G_MASK) >> 8, p >> 16, 255);
            SDL_RenderDrawPoint(renderer, x, y);

            x++;

        }

        x = 0;
        y++;

    }

    SDL_RenderPresent(renderer);


    return true;
}
