// SPDX-License-Identifier: MIT
//
// Game.hpp - top-level game class owning all systems and the loop.

#pragma once

#include "CupStack.hpp"
#include "CustomerLine.hpp"
#include "IngredientCarousel.hpp"
#include "RecipeBook.hpp"
#include "Types.hpp"

#include "raylib.h"

#include <string>
#include <vector>

namespace cha_craft {

class Game {
public:
    Game();
    ~Game();

    // Entry point. Initializes raylib, runs the loop, tears down.
    void Run();

private:
    // Per-frame steps for each game state.
    void UpdatePlaying(float dt);
    void UpdateGameOver(float dt);
    void Render();

    // State transitions.
    void Reset();
    void OnGameOver();

    // Sub-steps of UpdatePlaying().
    void HandleSpawning(float dt);
    void HandleCarousel();
    void HandleCup();
    void HandleServe();
    void UpdateFeedback(float dt);

    // Helper to write to the leaderboard.
    void RecordScore(int score);

    // --- systems ---
    RecipeBook         recipes_;
    IngredientCarousel ingredients_;
    CupStack           cup_;
    CustomerLine       customers_;

    // --- misc state ---
    int          score_          = 0;
    int          lives_          = STARTING_LIVES;
    std::vector<int> leaderboard_;   // sorted desc, capped at LEADERBOARD_SIZE

    std::string feedbackMsg_;
    Color       feedbackColor_   = WHITE;
    float       feedbackTimer_   = 0.0f;

    float spawnTimer_    = 0.0f;
    float spawnInterval_ = INITIAL_SPAWN_INTERVAL;
    int   nextCustomerId_ = 1;

    GameState state_ = GameState::PLAYING;
};

} // namespace cha_craft
