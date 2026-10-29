#pragma once

#include <cstdint>

struct EuclideanResult {
    std::int64_t gcd; // TODO: change this into uint
    std::int64_t u;
    std::int64_t v;

    bool operator==(const EuclideanResult&) const = default;
};

EuclideanResult extended_euclidean_algorithm(std::uint64_t a, std::uint64_t b);