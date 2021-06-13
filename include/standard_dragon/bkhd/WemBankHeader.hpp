//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemBankHeader : public WemChunk {
    public:
        static constexpr uint32_t fourcc = DRAGON_MAGIC32('B', 'K', 'H', 'D');
        uint32_t version;
        uint32_t id;
        uint32_t language_id;

        explicit WemBankHeader(dragon::Array<uint8_t> &buffer) {
            uintptr_t offset = 0;

            version     = buffer.lpcast<uint32_t>(offset);
            id          = buffer.lpcast<uint32_t>(offset);
            language_id = buffer.lpcast<uint32_t>(offset);
        }
    };
} // namespace dragon::bkhd
