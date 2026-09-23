// SPDX-License-Identifier: MIT
//
// Customer.hpp - customer payload and priority comparison.
//
// PriorityScore() is the dynamic ranking function: VIPs get a base boost
// and urgency (patience burned) raises it further, so a VIP about to
// leave ranks above everyone else.
//
// CAUTION (DSA lesson): std::priority_queue does NOT support decrease-
// key. Patience ticks down every frame, so we keep std::vector<Customer>
// as the source of truth in CustomerLine and rebuild a fresh
// priority_queue on demand (O(n log n) for our small n).

#pragma once

#include "Types.hpp"

namespace cha_craft {

struct Customer {
    int          id;
    CustomerType type;
    std::string  recipeName;
    float        patience;     // seconds remaining
    float        maxPatience;  // seconds they started with

    float PriorityScore() const;
};

// MAX-heap: top() returns the highest priority. Returns true if `a`
// has lower priority than `b` (the "less-than" the heap uses).
struct CustomerPriorityCompare {
    bool operator()(const Customer& a, const Customer& b) const;
};

} // namespace cha_craft
