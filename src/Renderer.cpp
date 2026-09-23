// SPDX-License-Identifier: MIT
//
// Renderer.cpp - the only translation unit (other than Input.cpp) that
// includes raylib.h. Drawing layout numbers are preserved exactly from
// the original single-file version.

#include "cha_craft/Renderer.hpp"

#include <algorithm>
#include <string>

namespace cha_craft {

namespace {

// Convert our raylib-free ColorRGBA into a raylib Color at the call site.
inline Color ToRay(const ColorRGBA& c) {
    return Color{c.r, c.g, c.b, c.a};
}

inline Color WithA(const ColorRGBA& c, std::uint8_t a) {
    return Color{c.r, c.g, c.b, a};
}

} // namespace

void Renderer::DrawCustomerPanel(const std::vector<Customer>& customers,
                                 const RecipeBook& recipes) {
    DrawText("CUSTOMER LINE", CUSTOMER_PANEL_X + 10, 20, 22, DARKBROWN);
    for (std::size_t i = 0; i < customers.size(); ++i) {
        const Customer& c = customers[i];
        int y = CUSTOMER_PANEL_Y + static_cast<int>(i) * CUSTOMER_PANEL_ROW_H;
        Color boxColor = (c.type == CustomerType::VIP)
            ? Color{255, 223, 130, 255}
            : Color{230, 220, 205, 255};
        DrawRectangle(CUSTOMER_PANEL_X, y, CUSTOMER_PANEL_W, 90, boxColor);
        DrawRectangleLines(CUSTOMER_PANEL_X, y, CUSTOMER_PANEL_W, 90, DARKBROWN);

        std::string label = (c.type == CustomerType::VIP ? "[VIP] " : "[Reg] ") + c.recipeName;
        DrawText(label.c_str(), CUSTOMER_PANEL_X + 10, y + 8, 16, BLACK);

        // Patience bar: green -> yellow -> flashing red.
        float pct = c.patience / c.maxPatience;
        Color barColor = GREEN;
        if (pct < PATIENCE_WARN_THRESHOLD) barColor = ORANGE;
        if (pct < PATIENCE_DANGER_THRESHOLD) {
            bool flash = (static_cast<int>(GetTime() * PATIENCE_FLASH_HZ) % 2) == 0;
            barColor = flash ? RED : Color{120, 20, 20, 255};
        }
        DrawRectangle(CUSTOMER_PANEL_X + 10, y + 55, 260, 16, Color{200, 200, 200, 255});
        DrawRectangle(CUSTOMER_PANEL_X + 10, y + 55,
                      static_cast<int>(260 * std::max(0.0f, pct)), 16, barColor);
        DrawRectangleLines(CUSTOMER_PANEL_X + 10, y + 55, 260, 16, BLACK);

        // Required ingredients listed under the bar.
        std::string ingList;
        for (const auto& ing : recipes.IngredientsFor(c.recipeName)) ingList += ing + " ";
        DrawText(ingList.c_str(), CUSTOMER_PANEL_X + 10, y + 74, 12, DARKGRAY);
    }
}

void Renderer::DrawCup(const CupStack& cup, const IngredientCarousel& ingredients) {
    int layerH = CUP_H / MAX_CUP_LAYERS;

    DrawText("YOUR CUP", CUP_X + 20, CUP_TOP_Y - 40, 22, DARKBROWN);

    // Snapshot is top-first; reverse for bottom-up drawing.
    auto order = cup.Snapshot();
    std::reverse(order.begin(), order.end());

    for (std::size_t i = 0; i < order.size(); ++i) {
        int layerY = CUP_TOP_Y + CUP_H - static_cast<int>(i + 1) * layerH;
        DrawRectangle(CUP_X, layerY, CUP_W, layerH, ToRay(ingredients.ColorOf(order[i])));
        DrawRectangleLines(CUP_X, layerY, CUP_W, layerH, Color{80, 60, 40, 255});
    }

    // Cup outline (drawn after so it frames the liquid).
    DrawRectangleLines(CUP_X, CUP_TOP_Y, CUP_W, CUP_H, DARKBROWN);
    DrawRectangleLines(CUP_X - 3, CUP_TOP_Y - 3, CUP_W + 6, CUP_H + 6, DARKBROWN);

    DrawText(TextFormat("%d / %d layers", static_cast<int>(cup.Size()), MAX_CUP_LAYERS),
             CUP_X + 15, CUP_TOP_Y + CUP_H + 10, 16, DARKGRAY);
}

void Renderer::DrawCarousel(const IngredientCarousel& ingredients) {
    const auto& all = ingredients.All();
    int startX = SCREEN_W / 2 - static_cast<int>(all.size()) * CAROUSEL_SLOT_W / 2;
    for (std::size_t i = 0; i < all.size(); ++i) {
        int x = startX + static_cast<int>(i) * CAROUSEL_SLOT_W;
        bool sel = (static_cast<int>(i) == ingredients.SelectedIndex());
        DrawRectangle(x, CAROUSEL_Y, CAROUSEL_SLOT_W - 10, CAROUSEL_H, ToRay(all[i].color));
        DrawRectangleLines(x, CAROUSEL_Y, CAROUSEL_SLOT_W - 10, CAROUSEL_H, sel ? RED : BLACK);
        if (sel) DrawRectangleLines(x - 2, CAROUSEL_Y - 2, CAROUSEL_SLOT_W - 6, CAROUSEL_H + 4, RED);
        DrawText(all[i].name.c_str(), x + 2, CAROUSEL_Y + CAROUSEL_H + 4, 11, DARKBROWN);
    }
    DrawText("<- -> choose   ENTER add   BACKSPACE undo   S serve",
             SCREEN_W / 2 - 230, CAROUSEL_Y + 100, 16, DARKGRAY);
}

void Renderer::DrawHUD(int score, int lives, const std::vector<int>& leaderboard) {
    DrawText(TextFormat("SCORE: %d", score), HUD_X, 20, 24, DARKBROWN);
    DrawText(TextFormat("LIVES: %d", lives), HUD_X, 55, 20, MAROON);
    DrawText("LEADERBOARD", HUD_X, 100, 20, DARKBROWN);
    for (std::size_t i = 0; i < leaderboard.size(); ++i) {
        DrawText(TextFormat("%d. %d", static_cast<int>(i) + 1, leaderboard[i]),
                 HUD_X, 130 + static_cast<int>(i) * 24, 18, BLACK);
    }
}

void Renderer::DrawFeedback(const std::string& msg, Color color, float timer) {
    if (timer > 0.0f) {
        DrawText(msg.c_str(),
                 SCREEN_W / 2 - MeasureText(msg.c_str(), 26) / 2,
                 150, 26, color);
    }
}

void Renderer::DrawGameOver(int score, const std::vector<int>& leaderboard) {
    const char* title = "GAME OVER";
    DrawText(title, SCREEN_W / 2 - MeasureText(title, 50) / 2, 150, 50, MAROON);

    std::string finalScore = "Final Score: " + std::to_string(score);
    DrawText(finalScore.c_str(),
             SCREEN_W / 2 - MeasureText(finalScore.c_str(), 28) / 2, 230, 28, DARKBROWN);

    DrawText("LEADERBOARD", SCREEN_W / 2 - 80, 300, 24, DARKBROWN);
    for (std::size_t i = 0; i < leaderboard.size(); ++i) {
        std::string line = std::to_string(i + 1) + ". " + std::to_string(leaderboard[i]);
        DrawText(line.c_str(), SCREEN_W / 2 - 60, 335 + static_cast<int>(i) * 28, 20, BLACK);
    }

    const char* hint = "Press R to restart";
    DrawText(hint, SCREEN_W / 2 - MeasureText(hint, 20) / 2, 550, 20, DARKGRAY);
}

} // namespace cha_craft
