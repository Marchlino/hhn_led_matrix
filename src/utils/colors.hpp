#pragma once
#include <cstdint>


typedef uint_fast32_t RGB;


/**
 * @fn rgb
 * @brief Create a 32bit value from the given R, G and B values.
 * @param uint8_t Blue channel
 * @param uint8_t Green channel
 * @param uint8_t Red channel
 */
RGB rgb( uint8_t r, uint8_t g, uint8_t b );
