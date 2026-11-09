#pragma once
#include <cstdint>
#include <optional>
#include <string>

/**
 * @struct Value
 * @brief One token of a player request: either a number or a string.
 * Values are owned (no heap pointers), so a request vector frees itself.
 */
typedef struct v {

    std::optional<std::string> string;
    std::optional<uint8_t> number;

} Value;
