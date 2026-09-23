// SPDX-License-Identifier: MIT
#include "cha_craft/Customer.hpp"

namespace cha_craft {

float Customer::PriorityScore() const {
    // VIP base boost + urgency from patience burned.
    float typeWeight = (type == CustomerType::VIP) ? 1000.0f : 500.0f;
    float urgency    = (maxPatience - patience) * 10.0f;
    return typeWeight + urgency;
}

bool CustomerPriorityCompare::operator()(const Customer& a, const Customer& b) const {
    // MAX-heap: returns true when a has *lower* priority than b.
    return a.PriorityScore() < b.PriorityScore();
}

} // namespace cha_craft
