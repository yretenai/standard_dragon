//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"
#include "WemHierarchyShared.hpp"

namespace dragon::bkhd::hirc {
    class WemHierarchySwitch : public WemHierarchyChunk {
    public:
        const static WemHierarchyType type = WemHierarchyType::SwitchContainer;

        explicit WemHierarchySwitch(dragon::Array<uint8_t> &buffer, uintptr_t &offset, uint32_t &version) {
            params     = NodeBaseParams(buffer, offset, version);
            group_type = buffer.lpcast<uint8_t>(offset);
            group_id   = buffer.lpcast<uint32_t>(offset);
            default_id = buffer.lpcast<uint32_t>(offset);
            validation = buffer.lpcast<uint8_t>(offset);
            auto count = buffer.lpcast<uint32_t>(offset);
            if (count > 0) {
                children = buffer.lpcast<uint32_t>(offset, count);
            }
        };

        NodeBaseParams params;
        uint8_t group_type  = 0;
        uint32_t group_id   = 0;
        uint32_t default_id = 0;
        uint8_t validation  = 0;
        dragon::Array<uint32_t> children;
    };
} // namespace dragon::bkhd::hirc
