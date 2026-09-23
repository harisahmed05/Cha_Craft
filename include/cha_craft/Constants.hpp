// SPDX-License-Identifier: MIT
//
// Constants.hpp - central tuning knobs for Cha Craft.
//
// Every magic number that previously lived inline in main.cpp is gathered
// here so the gameplay can be tuned without hunting through code.

#pragma once

namespace cha_craft {

// --- Window --------------------------------------------------------------
constexpr int SCREEN_W = 1280;
constexpr int SCREEN_H = 720;

// --- Game limits ---------------------------------------------------------
constexpr int  MAX_CUP_LAYERS = 6;
constexpr int  MAX_CUSTOMERS  = 6;
constexpr int  STARTING_LIVES = 3;
constexpr int  LEADERBOARD_SIZE = 5;

// --- Customer spawning ---------------------------------------------------
constexpr float INITIAL_SPAWN_INTERVAL = 3.5f;     // seconds
constexpr float MIN_SPAWN_INTERVAL     = 3.0f;     // seconds (lower bound)
constexpr int   SPAWN_JITTER_PERCENT   = 300;      // /100 added to MIN
constexpr int   VIP_CHANCE_PERCENT     = 20;       // 0..100
constexpr float REGULAR_PATIENCE_SEC   = 26.0f;
constexpr float VIP_PATIENCE_SEC       = 16.0f;

// --- Scoring -------------------------------------------------------------
constexpr int  REGULAR_REWARD    = 50;
constexpr int  VIP_REWARD        = 100;
constexpr float PATIENCE_BONUS_MULT = 2.0f;
constexpr int  WRONG_RECIPE_PENALTY = 5;
constexpr int  TIMEOUT_PENALTY      = 10;

// --- Patience bar visuals -----------------------------------------------
constexpr float PATIENCE_WARN_THRESHOLD = 0.55f;   // yellow
constexpr float PATIENCE_DANGER_THRESHOLD = 0.25f;  // flashing red
constexpr float PATIENCE_FLASH_HZ = 6.0f;

// --- HUD layout ---------------------------------------------------------
constexpr int   CUSTOMER_PANEL_X     = 20;
constexpr int   CUSTOMER_PANEL_W     = 280;
constexpr int   CUSTOMER_PANEL_Y     = 60;
constexpr int   CUSTOMER_PANEL_ROW_H = 100;

constexpr int   CUP_W      = 180;
constexpr int   CUP_H      = 300;
constexpr int   CUP_X      = SCREEN_W / 2 - CUP_W / 2;
constexpr int   CUP_TOP_Y  = 200;

constexpr int   CAROUSEL_Y     = 600;
constexpr int   CAROUSEL_SLOT_W = 90;
constexpr int   CAROUSEL_H     = 70;

constexpr int   HUD_X = SCREEN_W - 260;

} // namespace cha_craft
