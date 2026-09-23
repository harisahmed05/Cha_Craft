// SPDX-License-Identifier: MIT
//
// IngredientCarousel.hpp - the std::vector<Ingredient> wrapper.
//
// operator[]/random access is O(1). The player cycles through a fixed,
// ordered set of ingredients with Left/Right arrows, so contiguous
// storage + O(1) indexing is the natural fit.

#pragma once

#include "Types.hpp"

#include <string>
#include <vector>

namespace cha_craft {

class IngredientCarousel {
public:
    IngredientCarousel();

    std::size_t Size() const { return ingredients_.size(); }
    int  SelectedIndex() const { return selected_; }

    const Ingredient& Current() const;

    // Wrap-around navigation.
    void SelectNext();
    void SelectPrev();

    // Linear scan over a tiny fixed list - O(1) in practice.
    ColorRGBA ColorOf(const std::string& name) const;

    const std::vector<Ingredient>& All() const { return ingredients_; }

private:
    std::vector<Ingredient> ingredients_;
    int                     selected_ = 0;
};

} // namespace cha_craft
