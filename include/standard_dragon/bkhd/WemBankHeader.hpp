//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemBankHeader : public WemChunk {
    public:
        const static uint32_t fourcc = DRAGON_MAGIC32('B', 'K', 'H', 'D');
        uint32_t version;
        uint32_t id;

        explicit WemBankHeader(dragon::Array<uint8_t> &buffer) {
            version = buffer.cast<uint32_t>(0);
            id      = buffer.cast<uint32_t>(4);
        }
    };
} // namespace dragon::bkhd
