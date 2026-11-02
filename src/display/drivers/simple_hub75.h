#ifndef SIMPLE_HUB75_H
#define SIMPLE_HUB75_H

#include <stdint.h>

/**
 * @defgroup HUB75_DIMENSIONS Matrix Dimensions Configuration
 * @brief Layout settings for the main game board and secondary leaderboard displays.
 * @{
 */

/** @brief Total pixel width of the combined main gameboard display layout. */
#define WIDTH 64

/** @brief Total pixel height of the combined main gameboard display layout. */
#define HEIGHT 128

/** @brief Pixel width of the standalone leaderboard display matrix module. */
#define LEAD_WIDTH 32

/** @brief Pixel height of the standalone leaderboard display matrix module. */
#define LEAD_HEIGHT 32

/** @} */

/**
 * @defgroup HUB75_PUBLIC_API Public Driver Interface
 * @brief Core function entry points for controlling the HUB75 display system.
 * @{
 */

/**
 * @brief Hooks the pigpio kernel layer and establishes target panel output environments.
 */
void hub75_init(void);

/**
 * @brief Pushes structured component color values out into application target buffer locations.
 * @param x Horizontal pixel position index.
 * @param y Vertical pixel position index.
 * @param r Red subpixel channel intensity.
 * @param g Green subpixel channel intensity.
 * @param b Blue subpixel channel intensity.
 */
void hub75_set_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Submits work allocations for presentation on downstream physical LED panel matrix elements.
 */
void hub75_present_frame(void);

/**
 * @brief Maps high-score indicators out using internal 90-degree angular array shifting tricks.
 * @param x Horizontal positioning parameter.
 * @param y Vertical positioning parameter.
 * @param r Red channel payload value.
 * @param g Green channel payload value.
 * @param b Blue channel payload value.
 */
void leaderboard_set_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Safely shuts down driver tasks, joins backgrounds, and stops pin drive configurations.
 */
void hub75_cleanup(void);


#endif 