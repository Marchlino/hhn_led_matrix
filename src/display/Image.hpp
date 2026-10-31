#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "../utils/colors.hpp"

/**
 * @class Image
 * @desc Represent an image in memory. It allow to easily draw shapes and text using screen space coordinates.
 */
class Image {

    private:

        int size_x;
        int size_y;
        std::vector<RGB> pixels;
        RGB drawing_color;
        float view_unit_x;
        float view_unit_y;
        FT_Library library;
        FT_Face face;
        bool fontLoaded;

    public:

        Image(int size_x, int size_y);

        bool loadFont(std::string path);

        /**
         * @fn getPixelAt
         * @desc Return pixel at x, y
         */
        RGB getPixelAt(int x, int y);

        /**
         * @fn getPixelInView
         * @desc Return pixel at vw, vh
         */
        RGB getPixelInView(float vw, float vh);

        /**
         * @fn setPixelAt
         * @desc Color pixel at x, y
         */
        bool setPixelAt(int x, int y);

        /**
         * @fn setDrawingColor
         * @desc Change the drawing color
         */
        bool setDrawingColor(RGB color);

        /**
         * @fn clear
         * @desc Clear the Image to color 0
         */
        bool clear();

        /**
         * @fn weigthUnit
         * @desc Return the value if 1 pixel in screen space for the x axis
         */
        float weigthUnit();


        /**
         * @fn heigthUnit
         * @desc Return the value if 1 pixel in screen space for the y axis
         */
        float heigthUnit();

        /**
         * @fn drawGlyph
         * @desc Display a glyph at defined coordinates
         */
        bool drawGlyph(std::string glyph, int x, int y, int size_x, int size_y);

        /**
         * @fn drawText
         * @desc Display text at defined coordinates
         */
        bool drawText(std::string text, float vw, float vh, float weight, float height);

        /**
         * @fn drawRect
         * @desc Display a rectangle at defined coordinates
         */
        bool drawRect(float vw, float vh, float weight, float height);

        /**
         * @fn drawCircle
         * @desc Display a circle at defined coordinates
         */
        bool drawCircle(float vw, float vh, float radius);

        /**
         * @fn drawTriangle
         * @desc Display a triangle at defined coordinates
         */
        bool drawTriangle(float vw1, float vh1, float vw2, float vh2, float vw3, float vh3);

};
