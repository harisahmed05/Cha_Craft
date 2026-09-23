// SPDX-License-Identifier: MIT
//
// Game.cpp - top-level game loop, state transitions, and orchestration of
// the systems declared in include/cha_craft/.

#include "cha_craft/Game.hpp"

#include "cha_craft/Input.hpp"
#include "cha_craft/Renderer.hpp"

#include "raylib.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace cha_craft {

Game::Game() = default;
Game::~Game() = default;

void Game::Run() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    InitWindow(SCREEN_W, SCREEN_H, "Cha Craft: The Tea Lounge");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (state_ == GameState::PLAYING) {
            UpdatePlaying(dt);
        } else {
            UpdateGameOver(dt);
        }
        Render();
    }

    CloseWindow();
}

// ---------------- Update ------------------------------------------------

void Game::UpdatePlaying(float dt) {
    HandleSpawning(dt);

    int walkedOut = customers_.Tick(dt);
    for (int i = 0; i < walkedOut; ++i) {
        --lives_;
        score_ = std::max(0, score_ - TIMEOUT_PENALTY);
    }

    if (lives_ <= 0) {
        OnGameOver();
        UpdateFeedback(dt);
        return;
    }

    HandleCarousel();
    HandleCup();
    HandleServe();
    UpdateFeedback(dt);
}

void Game::UpdateGameOver(float /*dt*/) {
    if (PollAction() == Action::Restart) Reset();
}

void Game::HandleSpawning(float dt) {
    spawnTimer_ += dt;
    if (spawnTimer_ >= spawnInterval_) {
        spawnTimer_ = 0.0f;
        spawnInterval_ = MIN_SPAWN_INTERVAL +
            static_cast<float>(std::rand() % SPAWN_JITTER_PERCENT) / 100.0f;

        const auto& names = recipes_.Names();
        const std::string& pick = names[std::rand() % names.size()];
        bool isVIP = (std::rand() % 100) < VIP_CHANCE_PERCENT;
        float patience = isVIP ? VIP_PATIENCE_SEC : REGULAR_PATIENCE_SEC;
        customers_.Spawn(pick, isVIP ? CustomerType::VIP : CustomerType::REGULAR,
                         patience, nextCustomerId_++);
    }
}

void Game::HandleCarousel() {
    Action a = PollAction();
    if (a == Action::ChooseNext) ingredients_.SelectNext();
    else if (a == Action::ChoosePrev) ingredients_.SelectPrev();
}

void Game::HandleCup() {
    Action a = PollAction();
    if (a == Action::AddIngredient) {
        if (cup_.CanPush()) cup_.Push(ingredients_.Current().name);
    } else if (a == Action::Undo) {
        cup_.Pop();
    }
}

void Game::HandleServe() {
    if (PollAction() != Action::Serve) return;
    if (customers_.IsEmpty()) return;

    // Cup snapshot, sorted to compare as multisets regardless of push order.
    auto cupSorted = cup_.Snapshot();
    std::sort(cupSorted.begin(), cupSorted.end());

    auto result = customers_.ServeHighestPriority(recipes_, cupSorted);

    if (result.kind == ServeResult::Kind::MATCH) {
        score_ += result.reward;
        feedbackMsg_   = "Order matched! +" + std::to_string(result.reward);
        feedbackColor_ = GREEN;
    } else if (result.kind == ServeResult::Kind::MISMATCH) {
        score_ = std::max(0, score_ + result.reward); // result.reward is negative
        feedbackMsg_   = "Wrong recipe! " + std::to_string(result.reward);
        feedbackColor_ = RED;
    } else {
        return; // EMPTY - shouldn't reach here due to guard above
    }

    // Cup is used up either way (served or spilled).
    cup_.Reset();
    feedbackTimer_ = 1.5f;
}

void Game::UpdateFeedback(float dt) {
    if (feedbackTimer_ > 0.0f) feedbackTimer_ -= dt;
}

void Game::OnGameOver() {
    state_ = GameState::GAME_OVER;
    RecordScore(score_);
}

void Game::RecordScore(int score) {
    leaderboard_.push_back(score);
    std::sort(leaderboard_.begin(), leaderboard_.end(), std::greater<int>());
    if (leaderboard_.size() > LEADERBOARD_SIZE) leaderboard_.resize(LEADERBOARD_SIZE);
}

void Game::Reset() {
    cup_.Reset();
    customers_.Clear();
    score_         = 0;
    lives_         = STARTING_LIVES;
    spawnTimer_    = 0.0f;
    spawnInterval_ = INITIAL_SPAWN_INTERVAL;
    feedbackMsg_.clear();
    feedbackTimer_ = 0.0f;
    state_         = GameState::PLAYING;
}

// ---------------- Render ------------------------------------------------

void Game::Render() {
    BeginDrawing();
    ClearBackground(Color{245, 235, 220, 255}); // warm cream

    if (state_ == GameState::PLAYING) {
        Renderer::DrawCustomerPanel(customers_.All(), recipes_);
        Renderer::DrawCup(cup_, ingredients_);
        Renderer::DrawCarousel(ingredients_);
        Renderer::DrawHUD(score_, lives_, leaderboard_);
        Renderer::DrawFeedback(feedbackMsg_, feedbackColor_, feedbackTimer_);
    } else {
        Renderer::DrawGameOver(score_, leaderboard_);
    }

    EndDrawing();
}

} // namespace cha_craft
