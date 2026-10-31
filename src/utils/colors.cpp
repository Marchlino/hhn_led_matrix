#include <cstdint>

#include "colors.hpp"


RGB rgb( uint8_t r, uint8_t g, uint8_t b ) {

    RGB result = ( uint_fast32_t )r | ( uint_fast32_t )g << 8 | ( uint_fast32_t )b << 16;
    return result;

}
