//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"
#include "WemHierarchyShared.hpp"

namespace dragon::bkhd::hirc {
    class WemHierarchyContainer : public WemHierarchyChunk {
    public:
#pragma pack(push, 1)
        using PlaylistItem = struct PLAYLIST_ITEM {
            uint32_t id;
            int32_t weight;
        };
#pragma pack(pop)

        const static WemHierarchyType type = WemHierarchyType::Container;

        explicit WemHierarchyContainer(dragon::Array<uint8_t> &buffer, uintptr_t &offset) {
            params             = NodeBaseParams(buffer, offset);
            loop               = buffer.lpcast<ValueMod<uint16_t>>(offset);
            transition         = buffer.lpcast<ValueMod<float>>(offset);
            avoid_repeat_count = buffer.lpcast<uint16_t>(offset);
            transition_mode    = buffer.lpcast<uint8_t>(offset);
            random_mode        = buffer.lpcast<uint8_t>(offset);
            mode               = buffer.lpcast<uint8_t>(offset);
            flags              = buffer.lpcast<uint8_t>(offset);
            auto count         = buffer.lpcast<uint32_t>(offset);
            children           = buffer.lpcast<uint32_t>(offset, count);
            count              = buffer.lpcast<uint16_t>(offset);
            playlist           = buffer.lpcast<PlaylistItem>(offset, count);
        };

        NodeBaseParams params;
        ValueMod<uint16_t> loop;
        ValueMod<float> transition;
        uint16_t avoid_repeat_count;
        uint8_t transition_mode;
        uint8_t random_mode;
        uint8_t mode;
        uint8_t flags;
        dragon::Array<uint32_t> children;
        dragon::Array<PlaylistItem> playlist;
    };
} // namespace dragon::bkhd::hirc
