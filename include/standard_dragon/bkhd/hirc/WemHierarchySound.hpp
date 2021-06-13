//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"
#include "WemHierarchyShared.hpp"

namespace dragon::bkhd::hirc {
    class WemHierarchySound : public WemHierarchyChunk {
    public:
        const static WemHierarchyType type = WemHierarchyType::Sound;

        explicit WemHierarchySound(dragon::Array<uint8_t> &buffer, uintptr_t &offset, uint32_t &version) {
            source = SourceInfo(buffer, offset);
        };

        SourceInfo source;
    };
} // namespace dragon::bkhd::hirc
