//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemEffectPipeline : public WemChunk {
    public:
        static constexpr uint32_t fourcc = DRAGON_MAGIC32('F', 'X', 'P', 'R');

        explicit WemEffectPipeline(dragon::Array<uint8_t> &buffer) { }
    };
} // namespace dragon::bkhd
