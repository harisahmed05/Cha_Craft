// SPDX-License-Identifier: MIT
#include "cha_craft/IngredientCarousel.hpp"

namespace cha_craft {

namespace {
// Convenience constructor for the {r,g,b,a} literal in the initializer.
inline ColorRGBA rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) {
    return ColorRGBA{r, g, b, a};
}
} // namespace

IngredientCarousel::IngredientCarousel()
    : ingredients_({{"Black Tea", rgba(101, 67, 33)},
                    {"Milk",      rgba(255, 253, 240)},
                    {"Boba",      rgba(40, 20, 10)},
                    {"Sugar",     rgba(250, 240, 200)},
                    {"Matcha",    rgba(136, 176, 75)},
                    {"Lemon",     rgba(255, 230, 80)},
                    {"Taro",      rgba(190, 150, 210)}}) {}

const Ingredient& IngredientCarousel::Current() const {
    return ingredients_[static_cast<std::size_t>(selected_)];
}

void IngredientCarousel::SelectNext() {
    const int n = static_cast<int>(ingredients_.size());
    selected_ = (selected_ + 1 + n) % n;
}

void IngredientCarousel::SelectPrev() {
    const int n = static_cast<int>(ingredients_.size());
    selected_ = (selected_ - 1 + n) % n;
}

ColorRGBA IngredientCarousel::ColorOf(const std::string& name) const {
    for (const auto& ing : ingredients_) {
        if (ing.name == name) return ing.color;
    }
    return rgba(130, 130, 130); // GRAY fallback
}

} // namespace cha_craft
