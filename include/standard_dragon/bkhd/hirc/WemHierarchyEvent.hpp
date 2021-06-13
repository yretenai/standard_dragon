//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"
#include "WemHierarchyShared.hpp"

namespace dragon::bkhd::hirc {
    class WemHierarchyEvent : public WemHierarchyChunk {
    public:
        const static WemHierarchyType type = WemHierarchyType::Event;

        explicit WemHierarchyEvent(dragon::Array<uint8_t> &buffer, uintptr_t &offset, uint32_t &version) {
            auto count = buffer.lpcast<uint8_t>(offset);
            if (count > 0) {
                actions = buffer.lpcast<uint32_t>(offset, count);
            }
        };

        dragon::Array<uint32_t> actions;
    };
} // namespace dragon::bkhd::hirc
