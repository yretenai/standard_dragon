//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemSoundTypeId : public WemChunk {
    public:
        const static uint32_t fourcc = DRAGON_MAGIC32('S', 'T', 'I', 'D');

        explicit WemSoundTypeId(dragon::Array<uint8_t> &buffer) { }
    };
} // namespace dragon::bkhd
