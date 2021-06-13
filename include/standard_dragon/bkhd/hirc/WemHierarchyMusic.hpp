//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"
#include "WemHierarchyShared.hpp"

namespace dragon::bkhd::hirc {
    class WemHierarchyMusic : public WemHierarchyChunk {
    public:
        static constexpr WemHierarchyType type = WemHierarchyType::MusicContainer;

        explicit WemHierarchyMusic(dragon::Array<uint8_t> &buffer, uintptr_t &offset, uint32_t &version) {
            flags      = buffer.lpcast<uint8_t>(offset);
            params     = NodeBaseParams(buffer, offset, version);
            auto count = buffer.lpcast<uint32_t>(offset);
            if (count > 0) {
                children = buffer.lpcast<uint32_t>(offset, count);
            }
        };

        uint8_t flags;
        NodeBaseParams params;
        dragon::Array<uint32_t> children;
    };
} // namespace dragon::bkhd::hirc
