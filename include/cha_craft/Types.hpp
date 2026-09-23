// SPDX-License-Identifier: MIT
//
// Types.hpp - shared domain types (no raylib dependency).
//
// `Ingredient::color` is a plain {r,g,b,a} struct so domain headers stay
// raylib-free. The renderer's only contact with raylib is at the call
// site where we assign Ingredient.color to raylib's Color (the layouts
// are bit-compatible).

#pragma once

#include <cstdint>
#include <string>

namespace cha_craft {

// Customer tier affects priority weight and reward magnitude.
enum class CustomerType {
    REGULAR,
    VIP
};

// Top-level game state machine.
enum class GameState {
    PLAYING,
    GAME_OVER
};

// Plain RGBA color so this header is independent of raylib.
struct ColorRGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
};

// A single ingredient available on the carousel.
struct Ingredient {
    std::string name;
    ColorRGBA   color;
};

} // namespace cha_craft
