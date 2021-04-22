//
// Licensed as public domain.
// https://tools.ietf.org/html/draft-eastlake-fnv-17
//

#pragma once

#include <cstdint>

namespace dragon::hash {
    static const uint32_t FNV_PRIME_32 = 0x01000193;
    static const uint32_t FNV_BASIS_32 = 0x811C9DC5;
    static const uint64_t FNV_PRIME_64 = 0x00000100000001B3;
    static const uint64_t FNV_BASIS_64 = 0xCBF29CE484222325;

    uint32_t fnv1a32(const uint8_t* buf, size_t size, uint32_t bits = 32, uint32_t basis = FNV_BASIS_32, uint32_t prime = FNV_PRIME_32) {
        for(size_t i = 0; i < size; ++i) {
            basis ^= buf[i];
            basis *= prime;
        }

        if(bits >= 32) return basis;
        return basis & ((1 << bits) - 1);
    }

    uint64_t fnv1a64(const uint8_t* buf, size_t size, uint64_t bits = 64, uint64_t basis = FNV_BASIS_64, uint64_t prime = FNV_PRIME_64) {
        for(size_t i = 0; i < size; ++i) {
            basis ^= buf[i];
            basis *= prime;
        }
        if(bits >= 64) return basis;
        return basis & ((1 << bits) - 1);
    }

    uint32_t fnv1_32(const uint8_t* buf, size_t size, uint32_t bits = 32, uint32_t basis = FNV_BASIS_32, uint32_t prime = FNV_PRIME_32) {
        for(size_t i = 0; i < size; ++i) {
            basis *= prime;
            basis ^= buf[i];
        }
        if(bits >= 32) return basis;
        return basis & ((1 << bits) - 1);
    }

    uint64_t fnv1_64(const uint8_t* buf, size_t size, uint32_t bits = 64, uint64_t basis = FNV_BASIS_64, uint64_t prime = FNV_PRIME_64) {
        for(size_t i = 0; i < size; ++i) {
            basis *= prime;
            basis ^= buf[i];
        }
        if(bits >= 64) return basis;
        return basis & ((1 << bits) - 1);
    }
}
