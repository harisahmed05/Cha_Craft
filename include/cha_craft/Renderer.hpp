// SPDX-License-Identifier: MIT
//
// Renderer.hpp - all raylib drawing calls live here.
//
// Every drawing function takes plain data (or read-only references to
// the systems) and emits raylib draw calls. This is the only
// translation unit (other than Input.cpp) that includes raylib.h, so
// swapping rendering backends touches one file.

#pragma once

#include "Constants.hpp"
#include "CupStack.hpp"
#include "Customer.hpp"
#include "IngredientCarousel.hpp"
#include "RecipeBook.hpp"
#include "Types.hpp"

#include "raylib.h"

#include <string>
#include <vector>

namespace cha_craft {

class Renderer {
public:
    // PLAYING screen.
    static void DrawCustomerPanel(const std::vector<Customer>& customers,
                                  const RecipeBook& recipes);
    static void DrawCup(const CupStack& cup, const IngredientCarousel& ingredients);
    static void DrawCarousel(const IngredientCarousel& ingredients);
    static void DrawHUD(int score, int lives, const std::vector<int>& leaderboard);
    static void DrawFeedback(const std::string& msg, Color color, float timer);

    // GAME_OVER screen.
    static void DrawGameOver(int score, const std::vector<int>& leaderboard);
};

} // namespace cha_craft
