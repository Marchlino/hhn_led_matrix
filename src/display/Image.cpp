#include <cstdint>
#include <cmath>
#include <string>
#include <algorithm>

#include "logger.h"
#include "Image.hpp"


Image::Image( int size_x, int size_y ) {

    this->_size_x = size_x;
    this->_size_y = size_y;
    this->_pixels = std::vector<RGB>( size_x * size_y );

    this->_view_unit_x = 1./this->_size_x;
    this->_view_unit_y = 1./this->_size_y;

    this->_drawing_color = 255;

    this->clear();

    this->_fontLoaded = false;

    int error = FT_Init_FreeType( &this->_library );
    if ( error ) {

        logger( 3, "An error occured during FreeType init" );

    }

}


bool Image::loadFont( std::string path ) {

    int error = FT_New_Face( this->_library, path.c_str(), 0, &this->_face );

    if ( error == FT_Err_Unknown_File_Format ) {

        logger( 3, "The provided font file is not in a known font format!");
        return false;

    } else if ( error ) {

        logger( 3, "Could not open the provided font file!" );
        return false;

    }

    logger( 0, "- Loaded font" );
    this->_fontLoaded = true;

    return true;

}


bool Image::setPixelAt( int x, int y ) {

    if ( x < 0 || x >= this->_size_x ) return false;
    if ( y < 0 || y >= this->_size_y ) return false;

    this->_pixels[x + y * this->_size_x] = this->_drawing_color;

    return true;

}


RGB Image::getPixelAt( int x, int y ) {

    if ( x < 0 || x >= this->_size_x ) return 0;
    if ( y < 0 || y >= this->_size_y ) return 0;

    return this->_pixels[x + y * this->_size_x];

}


RGB Image::getPixelInView( float vw, float vh ) {

    int x = ( int )( vw / this->_view_unit_x );
    int y = ( int )( vh / this->_view_unit_y );

    return this->_pixels[x + y * this->_size_x];

}


float Image::weigthUnit() {

    return this->_view_unit_x;

}


float Image::heigthUnit() {

    return this->_view_unit_y;

}

bool Image::setDrawingColor( RGB color ) {

    this->_drawing_color = color;
    return true;

}


bool Image::clear() {

    for ( int i = 0; i < this->_size_x * this->_size_y; ++i )
        this->_pixels[i] = 0;

    return true;

}


bool Image::drawGlyph( std::string glyph, int x, int y, int size_x, int size_y ) {

    if ( !this->_fontLoaded ) {

        logger( 3, "Attempted to draw glyph when no font was loaded!" );
        return false;

    }

    int error = FT_Set_Pixel_Sizes( this->_face, size_x, size_y );

    // If someone want to add real Unicode support...
    int glyph_index = FT_Get_Char_Index( this->_face, ( uint_fast32_t )glyph[0] );

    error = FT_Load_Glyph( this->_face, glyph_index, FT_LOAD_DEFAULT );

    if ( error ) {

        logger( 3, "Could not load glyph!" );
        return false;

    }

    error = FT_Render_Glyph( this->_face->glyph, FT_RENDER_MODE_MONO );

    if (error) {
        logger( 3, "Could not render glyph!" );
        return false;
    }

    // Place the glyph bitmap on our image
    FT_Bitmap* bitmap = &this->_face->glyph->bitmap;
    int bearingX = this->_face->glyph->bitmap_left;
    int bearingY = this->_face->glyph->bitmap_top;

    for ( int row = 0; row < bitmap->rows; row++ ) {

        for ( int col = 0; col < bitmap->width; col++ ) {

            // For monochrome bitmaps, each byte contains 8 pixels
            int byte_index = row * bitmap->pitch + ( col / 8 );
            int bit_index = 7 - ( col % 8 );

            // Extract the bit for this pixel
            uint8_t pixel_value = ( bitmap->buffer[byte_index] >> bit_index ) & 1;

            if ( pixel_value ) {

                int pixel_x = x + col;
                int pixel_y = ( y + row - bearingY ) + size_y;
                setPixelAt( pixel_x, pixel_y );

            }

        }

    }

    return true;

}


bool Image::drawText( std::string text, float vw, float vh, float weight, float height ) {

    if ( !this->_fontLoaded ) {

        logger( 3, "Attempted to draw text when no font was loaded!" );
        return false;

    }

    int pixel_x = ( int )( vw / this->_view_unit_x );
    int pixel_y = ( int )( vh / this->_view_unit_y );
    int pixel_size_x = ( int )( weight / this->_view_unit_x );
    int pixel_size_y = ( int )( height / this->_view_unit_y );

    int char_size = pixel_size_x/text.length();

    for ( int cursor = 0; cursor < text.length(); cursor++ ) {

        drawGlyph (std::string( 1, text[cursor] ), cursor * char_size + pixel_x, pixel_y, char_size, pixel_size_y );

    }

    return true;

}


bool Image::drawRect( float vw, float vh, float weight, float height ) {

    int pixel_x = ( int )( vw / this->_view_unit_x );
    int pixel_y = ( int )( vh / this->_view_unit_y );
    int pixel_size_x = ( int )( weight / this->_view_unit_x );
    int pixel_size_y = ( int )( height / this->_view_unit_y );

    for ( int l = 0; l < pixel_size_y; l++ ) {

        for( int k = 0; k < pixel_size_x; k++ ) {

            setPixelAt( pixel_x + k, pixel_y + l );

        }

    }

    return true;

}

bool Image::drawCircle( float vw, float vh, float radius ) {

    int pixel_x = ( int )( vw / this->_view_unit_x );
    int pixel_y = ( int )( vh / this->_view_unit_y );
    float pixel_size_radius = ( radius / this->_view_unit_x );
    float pixel_diameter = pixel_size_radius * 2;

    for ( float l = 0; l < pixel_diameter; l++ ) {

        for( float k = 0; k < pixel_diameter; k++ ) {

            // If pixel within the circle...
            if ( sqrt( pow( pixel_size_radius - l, 2 ) + pow( pixel_size_radius - k, 2 ) ) < pixel_size_radius ) {

                setPixelAt(pixel_x + k, pixel_y + l);

            }

        }

    }

    return true;

}


bool Image::drawTriangle( float vw1, float vh1, float vw2, float vh2, float vw3, float vh3 ) {

    int x1 = ( int )( vw1 / this->_view_unit_x ), y1 = ( int )( vh1 / this->_view_unit_y );
    int x2 = ( int )( vw2 / this->_view_unit_x ), y2 = ( int )( vh2 / this->_view_unit_y );
    int x3 = ( int )( vw3 / this->_view_unit_x ), y3 = ( int )( vh3 / this->_view_unit_y );

    int minx = std::min( {x1, x2, x3} ), maxx = std::max( {x1, x2, x3} );
    int miny = std::min( {y1, y2, y3} ), maxy = std::max( {y1, y2, y3} );

    auto sign = []( int px, int py, int ax, int ay, int bx, int by ) {

        return ( px - bx ) * ( ay - by ) - ( ax - bx ) * ( py - by );

    };

    for ( int y = miny; y <= maxy; y++ ) {

        for ( int x = minx; x <= maxx; x++ ) {

            int d1 = sign( x, y, x1, y1, x2, y2 );
            int d2 = sign( x, y, x2, y2, x3, y3 );
            int d3 = sign( x, y, x3, y3, x1, y1 );
            if ( ( d1 <= 0 && d2 <= 0 && d3 <= 0 ) || ( d1 >= 0 && d2 >= 0 && d3 >= 0 ) )
                setPixelAt( x, y );

        }

    }

    return true;

}
