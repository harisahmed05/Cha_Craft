// SPDX-License-Identifier: MIT
//
// RecipeBook.hpp - the std::map<string, vector<string>> wrapper.
//
// std::map is a balanced red-black tree, so find() is O(log n). Recipes
// are looked up by name whenever we need to check what ingredients a
// customer's order requires, which scales much better than a linear
// scan as the recipe book grows.

#pragma once

#include <map>
#include <string>
#include <vector>

namespace cha_craft {

class RecipeBook {
public:
    RecipeBook();

    // O(log n)
    const std::vector<std::string>& IngredientsFor(const std::string& recipeName) const;

    // All recipe names (random-access for spawning).
    const std::vector<std::string>& Names() const { return names_; }

    // Convenience: does a name exist?
    bool Contains(const std::string& recipeName) const;

private:
    std::map<std::string, std::vector<std::string>> book_;
    std::vector<std::string> names_;  // mirrors keys() for O(1) random pick
};

} // namespace cha_craft
