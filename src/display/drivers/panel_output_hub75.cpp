#include "panel_output.h"

/*
 * Description:
 * This block declares the low-level C driver functions responsible for 
 * directly interfacing with the physical HUB75 LED matrix hardware.
 */
extern "C" {
void hub75_init(void);
void hub75_set_pixel(std::uint16_t x, std::uint16_t y, std::uint8_t r, std::uint8_t g, std::uint8_t b);
void leaderboard_set_pixel(std::uint16_t x, std::uint16_t y, std::uint8_t r, std::uint8_t g, std::uint8_t b);
void hub75_present_frame(void);
void hub75_refresh(void);
void hub75_cleanup(void);
}

/*
 * Description:
 * Internal module state storing dimensions, fixed constraints, and 
 * initialization flags hidden from the global scope.
 */
namespace {

    int panel_width = 0;
    int panel_height = 0;
    constexpr int leaderboard_width = 32;  // Fixed dimensions for the sub-display
    constexpr int leaderboard_height = 32;
    bool initialized = false;

}  

/*
 * Description:
 * Public hardware wrapper interface implementation providing safe pixel mapping,
 * multi-display rendering, canvas clearing, and resource management.
 */
namespace panel_output {

    /*
     * Caches screen boundaries and boots up the hardware driver.
     */
    void initialize(int width, int height) {
        panel_width = width;
        panel_height = height;
        hub75_init();
        initialized = true;
    }

    /*
     * Draws a single RGB pixel on the main canvas with strict boundary validation.
     */
    void set_pixel(int x, int y, std::uint8_t red, std::uint8_t green, std::uint8_t blue) {
        if (!initialized || x < 0 || y < 0 || x >= panel_width || y >= panel_height) {
            return; // Intercept out-of-bounds parameters to prevent crashes
        }

        hub75_set_pixel(
            static_cast<std::uint16_t>(x),
            static_cast<std::uint16_t>(y),
            red,
            green,
            blue);
    }

    /*
     * Draws a single RGB pixel on the fixed 32x32 leaderboard display.
     */
    void set_leaderboard_pixel(int x, int y, std::uint8_t red, std::uint8_t green, std::uint8_t blue) {
        if (!initialized || x < 0 || y < 0 || x >= leaderboard_width || y >= leaderboard_height) {
            return; // Safely discard invalid coordinate requests
        }

        leaderboard_set_pixel(
            static_cast<std::uint16_t>(x),
            static_cast<std::uint16_t>(y),
            red,
            green,
            blue);
    }

    /*
     * Flushes the current frame buffer data to the physical LED panels.
     */
    bool present() {
        if (!initialized) {
            return false;
        }

        hub75_present_frame();
        return true;
    }

    /*
     * Wipes the main canvas back to total darkness (RGB 0,0,0).
     */
    void clear() {
        if (!initialized) {
            return;
        }

        for (int y = 0; y < panel_height; ++y) {
            for (int x = 0; x < panel_width; ++x) {
                set_pixel(x, y, 0, 0, 0);
            }
        }
    }

    /*
     * Wipes the leaderboard canvas back to total darkness (RGB 0,0,0).
     */
    void clear_leaderboard() {
        if (!initialized) {
            return;
        }

        for (int y = 0; y < leaderboard_height; ++y) {
            for (int x = 0; x < leaderboard_width; ++x) {
                set_leaderboard_pixel(x, y, 0, 0, 0);
            }
        }
    }

    /*
     * Gracefully clears both screens, renders a blank frame, and detaches the hardware driver.
     */
    void shutdown() {
        if (!initialized) {
            return;
        }

        clear();
        clear_leaderboard();
        present(); // Force blank frame onto screen instantly
        hub75_cleanup();
        initialized = false;
    }

    /*
     * Returns an informative string tag describing the target driver hardware.
     */
    std::string description() {
        return "real HUB75 hardware";
    }

}