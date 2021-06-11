//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemEnvironment : public WemChunk {
    public:
        const static uint32_t fourcc = DRAGON_MAGIC32('E', 'N', 'V', 'S');

        explicit WemEnvironment(dragon::Array<uint8_t> &buffer) { }
    };
} // namespace dragon::bkhd
