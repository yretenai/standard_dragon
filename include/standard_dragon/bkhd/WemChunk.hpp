//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "../dragon.hpp"
#include <cstdint>

namespace dragon::bkhd {
    class WemChunk {
    public:
        static constexpr uint32_t fourcc = 0;
        virtual ~WemChunk() noexcept = default;
    };
} // namespace dragon::bkhd
