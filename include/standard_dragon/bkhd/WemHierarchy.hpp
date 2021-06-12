//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

#include "hirc/WemHierarchyChunk.hpp"
#include "hirc/WemHierarchyUnimplemented.hpp"

namespace dragon::bkhd {
    class WemHierarchy : public WemChunk {
    public:
        const static uint32_t fourcc = DRAGON_MAGIC32('H', 'I', 'R', 'C');

        explicit WemHierarchy(dragon::Array<uint8_t> &buffer) {
            uintptr_t ptr = 0;
            auto count    = buffer.lpcast<uint32_t>(ptr);
            for (auto i = 0; i < count; ++i) {
                auto type         = buffer.lpcast<dragon::bkhd::hirc::WemHierarchyType>(ptr);
                auto size         = buffer.lpcast<uint32_t>(ptr);
                auto slice        = buffer.lpslice(ptr, size);
                uintptr_t loc_ptr = 0;
                auto id           = slice.lpcast<uint32_t>(loc_ptr);
                types[id]         = type;

                switch (type) {
                    case hirc::WemHierarchyType::Sound:
                        break;
                    case hirc::WemHierarchyType::Action:
                        break;
                    case hirc::WemHierarchyType::Event:
                        break;
                    case hirc::WemHierarchyType::Container:
                        break;
                    case hirc::WemHierarchyType::SwitchContainer:
                        break;
                    case hirc::WemHierarchyType::MusicSegment:
                        break;
                    case hirc::WemHierarchyType::MusicTrack:
                        break;
                    case hirc::WemHierarchyType::MusicSwitch:
                        break;
                    case hirc::WemHierarchyType::MusicContainer:
                        break;
                    default:
                        chunks[id] = std::make_shared<hirc::WemHierarchyUnimplemented>(slice, loc_ptr);
                        break;
                }
            }
        }

        std::map<uint32_t, std::shared_ptr<hirc::WemHierarchyChunk>> chunks;
        std::map<uint32_t, hirc::WemHierarchyType> types;

        template<typename T>
        typename std::enable_if<std::is_base_of<dragon::bkhd::hirc::WemHierarchyChunk, T>::value, std::shared_ptr<T>>::value get_chunk(uint32_t id) {
            if (!chunks.contains(id)) {
                return nullptr;
            }

            if (types[id] != T::TYPE) {
                return nullptr;
            }
            return std::reinterpret_pointer_cast<T>(chunks[id]);
        }

        dragon::bkhd::hirc::WemHierarchyType get_type(uint32_t id) {
            if (!types.contains(id)) {
                return dragon::bkhd::hirc::WemHierarchyType::None;
            }
            return types[id];
        }
    };
} // namespace dragon::bkhd
