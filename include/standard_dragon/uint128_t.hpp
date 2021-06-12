//
// Created by Lilith on 2021-05-17.
//

#pragma once

#include <cstdint>

namespace dragon {
    using uint128_t = struct uint128 {
        uint64_t lo;
        uint64_t hi;

        uint128(const uint128 &other) {
            lo = other.lo;
            hi = other.hi;
        }

        uint128(const uint64_t &low, const uint64_t &high) {
            lo = low;
            hi = high;
        }

        explicit uint128(const uint64_t &low) {
            lo = low;
            hi = 0;
        }

        explicit uint128() {
            hi = 0;
            lo = 0;
        };

        inline bool operator<(const uint128 &rhs) const {
            return (hi < rhs.hi) | ((hi == rhs.hi) & (lo < rhs.lo));
        }

        inline bool operator<=(const uint128 &rhs) const {
            return (hi < rhs.hi) | ((hi == rhs.hi) & (lo <= rhs.lo));
        }

        inline bool operator>(const uint128 &rhs) const {
            return !((hi < rhs.hi) | ((hi == rhs.hi) & (lo <= rhs.lo)));
        }

        inline bool operator>=(const uint128 &rhs) const {
            return !((hi < rhs.hi) | ((hi == rhs.hi) & (lo < rhs.lo)));
        }

        inline bool operator==(const uint128 &rhs) const {
            return lo == rhs.lo && hi == rhs.hi;
        }
    };
} // namespace dragon
