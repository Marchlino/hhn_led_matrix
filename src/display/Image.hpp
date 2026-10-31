#pragma once
#include <vector>
#include <string>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "../utils/colors.hpp"


/**
 * @class Image
 * @brief Represent an image in memory. It allow to easily draw shapes and text using screen space coordinates.
 * @param int Horizontale size of the Image in pixels.
 * @param int Vertical size of the Image in pixels.
 */
class Image {

    private:

        int _size_x;
        int _size_y;
        std::vector<RGB> _pixels;
        RGB _drawing_color;
        float _view_unit_x;
        float _view_unit_y;
        FT_Library _library;
        FT_Face _face;
        bool _fontLoaded;

    public:

        Image( int size_x, int size_y );

        /**
         * @fn loadFont
         * @brief Load a font from a font file for FreeType to use.
         * @param std::string Path to the font to load.
         */
        bool loadFont( std::string path );

        /**
         * @fn getPixelAt
         * @brief Return pixel at x, y.
         * @param int X position.
         * @param int Y position.
         */
        RGB getPixelAt( int x, int y );

        /**
         * @fn getPixelInView
         * @brief Return pixel at vw, vh.
         * @param float View weight.
         * @param float View height.
         */
        RGB getPixelInView( float vw, float vh );

        /**
         * @fn setPixelAt
         * @brief Color pixel at x, y.
         * @param int X position.
         * @param int Y position.
         */
        bool setPixelAt( int x, int y );

        /**
         * @fn setDrawingColor
         * @brief Change the drawing color.
         * @param uint32_t Color in RGB format.
         */
        bool setDrawingColor( RGB color );

        /**
         * @fn clear
         * @brief Clear the Image to color 0.
         */
        bool clear();

        /**
         * @fn weigthUnit
         * @brief Return the value if 1 pixel in screen space for the x axis.
         */
        float weigthUnit();


        /**
         * @fn heigthUnit
         * @brief Return the value if 1 pixel in screen space for the y axis.
         */
        float heigthUnit();

        /**
         * @fn drawGlyph
         * @brief Display a glyph at defined coordinates.
         * @param std::string The glyph to display.
         * @param int X position.
         * @param int Y position.
         * @param int Horizontal size.
         * @param int Vertical size.
         */
        bool drawGlyph( std::string glyph, int x, int y, int size_x, int size_y );

        /**
         * @fn drawText
         * @brief Display text at defined coordinates
         * @param std::string The text to display.
         * @param float View weight.
         * @param float View height.
         * @param float Horizontal size.
         * @param float Vertical size.
         */
        bool drawText( std::string text, float vw, float vh, float weight, float height );

        /**
         * @fn drawRect
         * @brief Display a rectangle at defined coordinates.
         * @param float View weight.
         * @param float View height.
         * @param float Horizontal size.
         * @param float Vertical size.
         */
        bool drawRect( float vw, float vh, float weight, float height );

        /**
         * @fn drawCircle
         * @brief Display a circle at defined coordinates.
         * @param float View weight.
         * @param float View height.
         * @param float Radiis of the circle.
         */
        bool drawCircle( float vw, float vh, float radius );

        /**
         * @fn drawTriangle
         * @brief Display a triangle at defined coordinates.
         * @param float View weight of the 1 point.
         * @param float View height of the 1 point.
         * @param float View weight of the 2 point.
         * @param float View height of the 2 point.
         * @param float View weight of the 3 point.
         * @param float View height of the 3 point.
         */
        bool drawTriangle( float vw1, float vh1, float vw2, float vh2, float vw3, float vh3 );

};
