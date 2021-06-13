//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemSoundTypeGroup : public WemChunk {
    public:
        static constexpr uint32_t fourcc = DRAGON_MAGIC32('S', 'T', 'M', 'G');

        explicit WemSoundTypeGroup(dragon::Array<uint8_t> &buffer) { }
    };
} // namespace dragon::bkhd
