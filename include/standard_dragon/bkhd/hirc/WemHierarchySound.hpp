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

        explicit WemHierarchySound(dragon::Array<uint8_t> &buffer, uintptr_t &offset) {
            source = SourceInfo(buffer, offset);
            params = NodeBaseParams(buffer, offset);
        };

        SourceInfo source;
        NodeBaseParams params;
    };
} // namespace dragon::bkhd::hirc