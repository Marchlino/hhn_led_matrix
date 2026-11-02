#ifndef PANEL_OUTPUT_H
#define PANEL_OUTPUT_H

#include <cstdint>
#include <string>

/**
 * @namespace panel_output
 * @brief Subsystem interface for managing and rendering pixel data to a HUB75 LED matrix and auxiliary sub-displays.
 * * This module provides a thread-unsafe abstraction layer over hardware registers and/or simulation 
 * backends. It handles safety-bounds validation, multi-display configurations, and state synchronization.
 */
namespace panel_output {

/**
 * @brief Allocates runtime configuration boundaries and initializes underlying display hardware/drivers.
 * @param width Absolute horizontal pixel dimension of the primary matrix canvas.
 * @param height Absolute vertical pixel dimension of the primary matrix canvas.
 * @note This must be called before executing any state-mutating draw or flush calls.
 */
void initialize(int width, int height);

/**
 * @brief Stages an RGB color value for a specific pixel coordinate on the primary LED display grid.
 * @param x Horizontal coordinate index (0-indexed, left-to-right).
 * @param y Vertical coordinate index (0-indexed, top-to-bottom).
 * @param red 8-bit red channel intensity value [0-255].
 * @param green 8-bit green channel intensity value [0-255].
 * @param blue 8-bit blue channel intensity value [0-255].
 * @note Operates with safety guard overrides; out-of-bounds coordinates are discarded silently.
 */
void set_pixel(int x, int y, std::uint8_t red, std::uint8_t green, std::uint8_t blue);

/**
 * @brief Stages an RGB color value for a specific coordinate on the auxiliary leaderboard sub-display.
 * @param x Horizontal coordinate index within the dedicated sub-display bounds.
 * @param y Vertical coordinate index within the dedicated sub-display bounds.
 * @param red 8-bit red channel intensity value [0-255].
 * @param green 8-bit green channel intensity value [0-255].
 * @param blue 8-bit blue channel intensity value [0-255].
 * @note Constrained independently by internal sub-display dimensions (e.g., hardcoded 32x32 runtime parameters).
 */
void set_leaderboard_pixel(int x, int y, std::uint8_t red, std::uint8_t green, std::uint8_t blue);

/**
 * @brief Commits all currently staged pixel transformations to the physical hardware registry or simulation logs.
 * @return true if the buffer flush and synchronization transaction completed without errors.
 */
bool present();

/**
 * @brief Synchronously flushes all coordinate points on the primary matrix back to baseline zero (Black/Off).
 * @note This modification is staged in-memory and will not be visible on screen until present() is called.
 */
void clear();

/**
 * @brief Synchronously flushes all coordinate points on the leaderboard sub-display back to baseline zero (Black/Off).
 * @note This modification is staged in-memory and will not be visible on screen until present() is called.
 */
void clear_leaderboard();

/**
 * @brief Executes a clean system deallocation protocol.
 * * Clears active frame states across all matrices, enforces a final dark frame flush to the display output, 
 * releases allocated lower-level C driver holds, and shifts the subsystem to an uninitialized state.
 */
void shutdown();

/**
 * @brief Retrieves technical runtime profiling metrics and diagnostic configurations.
 * @return A formatted std::string containing active targets, IO routes, or mock simulation file paths.
 */
std::string description();

}  

#endif // PANEL_OUTPUT_H