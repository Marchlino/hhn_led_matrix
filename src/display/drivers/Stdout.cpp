#include <stdio.h>

#include "Stdout.hpp"


bool Stdout::display( Image* img ) {

    printf( "\033[2J\033[1;1H" );

    int size_x = ( int )( 1. / ( img->weigthUnit() ) );
    int size_y = ( int )( 1. / ( img->heigthUnit() ) );

    for ( int l = 0; l <= size_y; l++ ) {

        for ( int k = 0; k <= size_x; k++ ) {

            if ( img->getPixelAt( k, l ) > 0 ) {

                printf( "##" );

            } else {

                printf( "  " );

            }

        }

        printf( "\n" );

    }

    return true;
}
