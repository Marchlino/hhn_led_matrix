#include "logger.h"
#include <thread>

#include "games/Game.hpp"
#include "games/MainMenu.hpp"
#include "display/Image.hpp"
#include "display/Display.hpp"
#include "display/drivers/Sdl.hpp"


using namespace std::this_thread;
using namespace std::chrono_literals;


int main( int argc, char** argv ) {


    if ( argc != 4 ) {

        logger( 3, "Missing parameters: [exe] <Weight> <Height> <ttf file>" );
        return 1;

    }

    int size_x = std::stoi( argv[1] );
    int size_y = std::stoi( argv[2] );
    std::string font_path = argv[3];

    logger( 0, "-- LED Matrix Start --" );

    std::vector<Game*> games;

    logger( 0, "- Loaded games" );

    MainMenu* menu = new MainMenu( games );

    logger( 0, "- Loaded Main Menu" );

    Image* screen = new Image( size_x, size_y );
    screen->loadFont( font_path );

    logger( 0, "- Loaded Image (screen)" );

    Display* display = new Sdl( 512, 512 );

    logger( 0, "- Loaded Display" );

    logger( 0, "-- LED Matrix loaded --" );

    while ( true ) {

        menu->update();
        menu->render( screen );

        // For instance we closed the Window
        if ( !display->display( screen ) ) {

            delete display;
            return 0;

        }

        //Just to avoid to use all of the CPU because we try to render at 65465 frames/seconds
        sleep_for( 1ms );

    }

}
