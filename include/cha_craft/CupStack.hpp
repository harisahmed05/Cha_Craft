// SPDX-License-Identifier: MIT
//
// CupStack.hpp - the std::stack<string> wrapper.
//
// A cup is built by adding ingredients one on top of another and
// "undo" means removing the most recently added one - classic LIFO,
// which is precisely what std::stack models. push/pop/top are all O(1).

#pragma once

#include "Constants.hpp"

#include <stack>
#include <string>
#include <vector>

namespace cha_craft {

class CupStack {
public:
    bool IsEmpty() const { return stack_.empty(); }
    std::size_t Size() const { return stack_.size(); }
    std::size_t MaxSize() const { return MAX_CUP_LAYERS; }

    bool CanPush() const { return Size() < MaxSize(); }
    void Push(const std::string& name);
    void Pop();
    void Reset();

    // Non-destructive snapshot. Returned in *top-first* order (the
    // most recently pushed ingredient is at index 0). Both the renderer
    // (which wants bottom-up) and the matcher (which sorts anyway) use
    // this so the snapshot logic lives in one place.
    std::vector<std::string> Snapshot() const;

private:
    std::stack<std::string> stack_;  // value-initialized empty
};

} // namespace cha_craft
