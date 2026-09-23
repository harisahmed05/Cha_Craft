// SPDX-License-Identifier: MIT
#include "cha_craft/CupStack.hpp"

namespace cha_craft {

void CupStack::Push(const std::string& name) {
    if (CanPush()) stack_.push(name);
}

void CupStack::Pop() {
    if (!stack_.empty()) stack_.pop();
}

void CupStack::Reset() {
    while (!stack_.empty()) stack_.pop();
}

std::vector<std::string> CupStack::Snapshot() const {
    // Walk a copy of the stack so we don't mutate it. Result is in
    // *top-first* order - the most recently pushed ingredient is at
    // index 0. The renderer reverses for bottom-up drawing; the matcher
    // sorts anyway.
    std::stack<std::string> tmp = stack_;
    std::vector<std::string> out;
    out.reserve(tmp.size());
    while (!tmp.empty()) {
        out.push_back(tmp.top());
        tmp.pop();
    }
    return out;
}

} // namespace cha_craft
