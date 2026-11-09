
#include "Sdl.hpp"
#include "logger.h"
#include <string>


Sdl::Sdl( Config* conf ) {

    this->_size_x = std::stoi( conf->get("sdl_x_size") );
    this->_size_y = std::stoi( conf->get("sdl_y_size") );
    this->_tex_x = std::stoi( conf->get("matrix_x_size") );
    this->_tex_y = std::stoi( conf->get("matrix_y_size") );
    this->_sdl_ok = false;

    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        logger(3, ("SDL could not start: " + std::string(SDL_GetError())).c_str() );
        return;
    }

    if ( SDL_CreateWindowAndRenderer( this->_size_x, this->_size_y, 0, &this->_window, &this->_renderer ) ) {
        logger(3, ("SDL create renderer & window: " + std::string(SDL_GetError())).c_str() );
        return;
    }

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" );
    this->_texture = SDL_CreateTexture( this->_renderer, SDL_PIXELFORMAT_XBGR8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        this->_tex_x, this->_tex_y );
    if ( !this->_texture ) {
        logger(3, ("SDL create texture: " + std::string(SDL_GetError())).c_str() );
        return;
    }

    this->_sdl_ok = true;
}

Sdl::~Sdl() {

    if ( this->_texture )  SDL_DestroyTexture( this->_texture );
    if ( this->_renderer ) SDL_DestroyRenderer( this->_renderer );
    if ( this->_window )   SDL_DestroyWindow( this->_window );
    SDL_Quit();

}

bool Sdl::display( Image* img ) {

    if ( !this->_sdl_ok ) return false;

    while ( SDL_PollEvent( &this->_event ) )
        if ( this->_event.type == SDL_QUIT )
            return false;

    void* raw;
    int pitch;
    if ( SDL_LockTexture( this->_texture, nullptr, &raw, &pitch ) != 0 )
        return false;

    const float ux = img->weigthUnit();
    const float uy = img->heigthUnit();

    for ( int y = 0; y < this->_tex_y; y++ ) {

        Uint32* row = reinterpret_cast<Uint32*>( static_cast<Uint8*>(raw) + y * pitch );
        const float l = ( y + 0.5f ) * uy;

        for ( int x = 0; x < this->_tex_x; x++ )
            row[x] = img->getPixelInView( ( x + 0.5f ) * ux, l );
    }

    SDL_UnlockTexture( this->_texture );
    SDL_RenderCopy( this->_renderer, this->_texture, nullptr, nullptr );
    SDL_RenderPresent( this->_renderer );

    return true;
}
