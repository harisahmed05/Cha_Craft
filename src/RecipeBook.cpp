// SPDX-License-Identifier: MIT
#include "cha_craft/RecipeBook.hpp"

namespace cha_craft {

RecipeBook::RecipeBook()
    : book_({{"Classic Milk Tea", {"Black Tea", "Milk", "Sugar"}},
             {"Boba Milk Tea",    {"Black Tea", "Milk", "Boba", "Sugar"}},
             {"Sweet Matcha",     {"Matcha", "Milk", "Sugar"}},
             {"Iced Lemon Tea",   {"Black Tea", "Lemon", "Sugar"}},
             {"Taro Delight",     {"Taro", "Milk", "Boba"}}}) {
    // Mirror keys for O(1) random pick during spawning.
    names_.reserve(book_.size());
    for (const auto& kv : book_) names_.push_back(kv.first);
}

const std::vector<std::string>& RecipeBook::IngredientsFor(const std::string& recipeName) const {
    // map::at throws if missing - the caller is expected to pass a
    // name from Names(). The static book is fixed so this never throws
    // in practice.
    return book_.at(recipeName);
}

bool RecipeBook::Contains(const std::string& recipeName) const {
    return book_.find(recipeName) != book_.end();
}

} // namespace cha_craft
