//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"
#include "WemHierarchyShared.hpp"

namespace dragon::bkhd::hirc {
    class WemHierarchyAction : public WemHierarchyChunk {
    public:
        const static WemHierarchyType type = WemHierarchyType::Action;

        explicit WemHierarchyAction(dragon::Array<uint8_t> &buffer, uintptr_t &offset) {
            action_type = buffer.lpcast<uint16_t>(offset);
            target      = buffer.lpcast<uint32_t>(offset);

            // todo.
        };

        uint16_t action_type;
        uint32_t target;
    };
} // namespace dragon::bkhd::hirc
