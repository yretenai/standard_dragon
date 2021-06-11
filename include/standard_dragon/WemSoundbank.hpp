//
// Created by Lilith on 2020-08-16.
//

#pragma once

#include "Array.hpp"
#include "bkhd/WemBankHeader.hpp"
#include "bkhd/WemData.hpp"
#include "bkhd/WemDataIndex.hpp"
#include "bkhd/WemEffectPipeline.hpp"
#include "bkhd/WemEnvironment.hpp"
#include "bkhd/WemHierarchy.hpp"
#include "bkhd/WemSoundTypeGroup.hpp"
#include "bkhd/WemSoundTypeId.hpp"
#include "dragon.hpp"
#include <cstdint>
#include <map>
#include <memory>

namespace dragon {
    class WemSoundbank {
    public:
#pragma pack(push, 1)
        typedef struct BNK_CHUNK_HEADER {
            uint32_t fourcc = 0;
            uint32_t size   = 0;
        } BnkChunkHeader;
        DRAGON_ASSERT(sizeof(BnkChunkHeader) == 8, "Bnk Chunk Header has an invalid size");
#pragma pack(pop)

        explicit WemSoundbank(const dragon::Array<uint8_t> &buffer) {
            base_stream   = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size());
            uintptr_t ptr = 0;
            while (ptr < base_stream->size()) {
                auto header           = base_stream->lpcast<BnkChunkHeader>(&ptr);
                chunks[header.fourcc] = std::pair<uintptr_t, BnkChunkHeader>(ptr, header);
                auto chunk            = get_chunk(header.fourcc);
                switch (header.fourcc) {
                    case dragon::bkhd::WemBankHeader::fourcc:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemBankHeader>(chunk);
                        break;
                    case dragon::bkhd::WemData::fourcc:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemData>(chunk);
                        break;
                    case dragon::bkhd::WemDataIndex::fourcc:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemDataIndex>(chunk);
                        break;
                    case dragon::bkhd::WemEffectPipeline::fourcc:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemEffectPipeline>(chunk);
                        break;
                    case dragon::bkhd::WemEnvironment::fourcc:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemEnvironment>(chunk);
                        break;
                    case dragon::bkhd::WemHierarchy::fourcc:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemHierarchy>(chunk);
                        break;
                    case dragon::bkhd::WemSoundTypeGroup::fourcc:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemSoundTypeGroup>(chunk);
                        break;
                    case dragon::bkhd::WemSoundTypeId::fourcc:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemSoundTypeId>(chunk);
                        break;
                    default:
                        DRAGON_ELOG("Unprocessed chunk " << header.fourcc);
                        break;
                }
                ptr += header.size;
            }
        }

        ~WemSoundbank() = default;

        std::shared_ptr<dragon::Array<uint8_t>> base_stream;
        std::map<uint32_t, std::pair<uintptr_t, BnkChunkHeader>> chunks;
        std::map<uint32_t, std::shared_ptr<dragon::bkhd::WemChunk>> parsed_chunks;

        dragon::Array<uint8_t> get_chunk(uint32_t fourcc) {
            std::pair<uintptr_t, BnkChunkHeader> pair = chunks[fourcc];
            return dragon::Array<uint8_t>(base_stream, pair.first, pair.second.size);
        }

        template<typename T>
        typename std::enable_if<std::is_base_of<dragon::bkhd::WemChunk, T>::value, std::shared_ptr<T>>::value get_chunk_impl() {
            if (!has_chunk_impl<T>()) {
                return nullptr;
            }
            return std::reinterpret_pointer_cast<T>(parsed_chunks[T::fourcc]);
        }

        bool has_chunk(uint32_t fourcc) { return chunks.find(fourcc) != chunks.end(); }

        template<typename T>
        typename std::enable_if<std::is_base_of<dragon::bkhd::WemChunk, T>::value, bool>::value has_chunk_impl() { return parsed_chunks.find(T::fourcc) != parsed_chunks.end(); }
    };
} // namespace dragon
