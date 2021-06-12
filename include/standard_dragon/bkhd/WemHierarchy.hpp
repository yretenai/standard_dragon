//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

#include "hirc/WemHierarchyType.hpp"

namespace dragon::bkhd {
    class WemHierarchy : public WemChunk {
    public:
        const static uint32_t fourcc = DRAGON_MAGIC32('H', 'I', 'R', 'C');

        explicit WemHierarchy(dragon::Array<uint8_t> &buffer) { }
    };
} // namespace dragon::bkhd
