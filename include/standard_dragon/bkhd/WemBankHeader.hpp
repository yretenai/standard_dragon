//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemBankHeader : public WemChunk {
    public:
        uint32_t version;
        uint32_t id;

        WemBankHeader(dragon::Array<uint8_t> buffer) {
            version = buffer.cast<uint32_t>(0);
            id      = buffer.cast<uint32_t>(4);
        }
    };
} // namespace dragon::bkhd
