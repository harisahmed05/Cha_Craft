// SPDX-License-Identifier: MIT
//
// Input.hpp - raylib-agnostic input vocabulary.
//
// The rest of the game code speaks in terms of `Action` enums rather
// than raw raylib keycodes. This means swapping raylib for SDL or any
// other backend later means editing only Input.cpp.

#pragma once

namespace cha_craft {

enum class Action {
    None,
    ChooseNext,    // ->  (or wrap-around right)
    ChoosePrev,    // <-  (or wrap-around left)
    AddIngredient, // ENTER / SPACE
    Undo,          // BACKSPACE
    Serve,         // S
    Restart        // R (only meaningful on GAME_OVER)
};

// Returns the highest-priority action whose key was pressed *this* frame.
// Returns Action::None if no action is currently active. Repeated polling
// is handled by raylib's IsKeyPressed, so each tap fires once.
Action PollAction();

} // namespace cha_craft
