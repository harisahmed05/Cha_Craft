// SPDX-License-Identifier: MIT
//
// main.cpp - the entire entry point.
//
// All interesting logic lives in cha_craft::Game. This file is just
// the bootstrapping shim that creates the Game and hands control to it.

#include "cha_craft/Game.hpp"

int main() {
    cha_craft::Game game;
    game.Run();
    return 0;
}
