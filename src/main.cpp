#include "logger.h"

#include "games/Game.hpp"
#include "games/MainMenu.hpp"
#include "games/Pong.hpp"
#include "display/Image.hpp"
#include "display/Display.hpp"
#include "display/drivers/Sdl.hpp"
#include "display/drivers/Stdout.hpp"
#include "utils/Config.hpp"
#include "network/Network.hpp"


using namespace std::this_thread;
using namespace std::chrono_literals;


bool end_program = false;
void endProgram(int sig) {
    end_program = true;
}


int main( int argc, char** argv ) {

    if ( argc != 2 ) {

        logger( 3, "Missing parameters: [exe] <config file>" );
        return 1;

    }

    Config* conf = new Config();
    if ( conf->parse( argv[1] ) ) return 1;

    std::string font_path = conf->get("font_path");

    logger( 0, "-- LED Matrix Start --" );

    std::vector<Game*> games = { new Pong() };

    logger( 0, "- Loaded games" );

    MainMenu* menu = new MainMenu( games );

    logger( 0, "- Loaded Main Menu" );

    Image* screen = new Image( std::stoi( conf->get("matrix_x_size") ), std::stoi( conf->get("matrix_y_size") ) );
    screen->loadFont( font_path );

    logger( 0, "- Loaded Image (screen)" );

    Display* display;
    if ( conf->get("driver") == "sdl" )
        display = new Sdl( conf );
    else if ( conf->get("driver") == "stdout" )
        display = new Stdout();

    logger( 0, "- Loaded Display" );

    Network* server = new Network( conf, menu );
    server->init();
    server->run();
    signal(SIGINT, endProgram);

    logger( 0, "- Server loaded & Running" );

    logger( 0, "-- LED Matrix loaded --" );

    while ( true ) {

        menu->update();
        menu->render( screen );
        server->sync();

        // For instance we closed the Window
        if ( !display->display( screen ) || end_program ) {

            // Server first, it stops Crow and removes players from the menu.
            delete server;
            delete display;
            delete menu;
            for ( Game* g: games )
                delete g;
            delete screen;
            delete conf;
            return 0;

        }

        //Just to avoid to use all of the CPU because we try to render at 65465 frames/seconds
        sleep_for( 1ms );

    }

    return 0;

}
