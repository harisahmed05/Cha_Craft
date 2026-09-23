// SPDX-License-Identifier: MIT
//
// CustomerLine.hpp - the std::vector<Customer> source of truth + on-
// demand priority_queue rebuild.
//
// We keep a vector (not a priority_queue) as the authoritative store
// because patience updates every frame and we need O(1) erase by id.
// When the player presses SERVE we rebuild a fresh priority_queue from
// the current state in O(n log n) - for our small n (<= 6) this is
// trivially cheap and keeps ordering provably correct.

#pragma once

#include "Customer.hpp"
#include "RecipeBook.hpp"

#include <optional>
#include <queue>
#include <vector>

namespace cha_craft {

// Result of attempting to serve the highest-priority customer.
struct ServeResult {
    enum class Kind { MATCH, MISMATCH, EMPTY };

    Kind         kind        = Kind::EMPTY;
    int          customerId  = -1;        // -1 if EMPTY
    CustomerType customerType = CustomerType::REGULAR;
    int          reward      = 0;          // net score delta (negative for penalty)
    bool         vip         = false;

    ServeResult() = default;
    ServeResult(Kind k, int id, CustomerType t, int r, bool isVip)
        : kind(k), customerId(id), customerType(t), reward(r), vip(isVip) {}
};

class CustomerLine {
public:
    CustomerLine();

    std::size_t Size() const { return customers_.size(); }
    bool IsEmpty() const { return customers_.empty(); }
    bool IsFull() const;
    const std::vector<Customer>& All() const { return customers_; }

    // Tick down patience for everyone. Returns the number of customers
    // who walked out (their patience ran out).
    int Tick(float dt);

    // Add a brand new customer. Caller chooses name/type/patience so
    // spawning policy lives in Game.
    void Spawn(const std::string& recipeName, CustomerType type, float patience, int id);

    // Pop the most-urgent customer, compare against the cup snapshot,
    // and either remove them (MATCH) or leave them in place and penalize
    // (MISMATCH). The cup is the caller's responsibility to reset.
    ServeResult ServeHighestPriority(const RecipeBook& recipes,
                                     const std::vector<std::string>& cupSnapshotSorted);

    void Clear();

private:
    std::vector<Customer> customers_;
};

} // namespace cha_craft
