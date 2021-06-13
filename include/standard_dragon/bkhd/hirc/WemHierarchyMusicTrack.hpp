//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"
#include "WemHierarchyShared.hpp"

namespace dragon::bkhd::hirc {
    class WemHierarchyMusicTrack : public WemHierarchyChunk {
    public:
        static constexpr WemHierarchyType type = WemHierarchyType::MusicTrack;

        explicit WemHierarchyMusicTrack(dragon::Array<uint8_t> &buffer, uintptr_t &offset, uint32_t &version) {
            flags      = buffer.lpcast<uint8_t>(offset);
            auto count = buffer.lpcast<uint32_t>(offset);
            if (count > 0) {
                sources.reserve(count);
                for (auto i = 0; i < count; ++i) {
                    sources.emplace_back(buffer, offset);
                }
            }
        };

        uint8_t flags;
        std::vector<SourceInfo> sources;
    };
} // namespace dragon::bkhd::hirc
