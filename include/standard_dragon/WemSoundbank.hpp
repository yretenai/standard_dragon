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

#define CAST_WEMBNK_CHUNK(target, chunk) (std::reinterpret_pointer_cast<target>(chunk).get())

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
        static constexpr uint32_t BKHD_FOURCC = DRAGON_MAGIC32('B', 'K', 'H', 'D');
        static constexpr uint32_t DATA_FOURCC = DRAGON_MAGIC32('D', 'A', 'T', 'A');
        static constexpr uint32_t DIDX_FOURCC = DRAGON_MAGIC32('D', 'I', 'D', 'X');
        static constexpr uint32_t FXPR_FOURCC = DRAGON_MAGIC32('F', 'X', 'P', 'R');
        static constexpr uint32_t ENVS_FOURCC = DRAGON_MAGIC32('E', 'N', 'V', 'S');
        static constexpr uint32_t HIRC_FOURCC = DRAGON_MAGIC32('H', 'I', 'R', 'C');
        static constexpr uint32_t STMG_FOURCC = DRAGON_MAGIC32('S', 'T', 'M', 'G');
        static constexpr uint32_t STID_FOURCC = DRAGON_MAGIC32('S', 'T', 'I', 'D');

        explicit WemSoundbank(const dragon::Array<uint8_t> &buffer) {
            base_stream   = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size(), true);
            uintptr_t ptr = 0;
            while (ptr < base_stream->size()) {
                auto header           = base_stream->lpcast<BnkChunkHeader>(&ptr);
                chunks[header.fourcc] = std::pair<uintptr_t, BnkChunkHeader>(ptr, header);
                switch (header.fourcc) {
                    case BKHD_FOURCC:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemBankHeader>(get_chunk(header.fourcc));
                        break;
                    case DATA_FOURCC:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemData>(get_chunk(header.fourcc));
                        break;
                    case DIDX_FOURCC:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemDataIndex>(get_chunk(header.fourcc));
                        break;
                    case FXPR_FOURCC:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemEffectPipeline>(get_chunk(header.fourcc));
                        break;
                    case ENVS_FOURCC:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemEnvironment>(get_chunk(header.fourcc));
                        break;
                    case HIRC_FOURCC:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemHierarchy>(get_chunk(header.fourcc));
                        break;
                    case STMG_FOURCC:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemSoundTypeGroup>(get_chunk(header.fourcc));
                        break;
                    case STID_FOURCC:
                        parsed_chunks[header.fourcc] = std::make_shared<dragon::bkhd::WemSoundTypeId>(get_chunk(header.fourcc));
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

        std::shared_ptr<dragon::bkhd::WemChunk> get_chunk_impl(uint32_t fourcc) {
            if (!has_chunk_impl(fourcc)) {
                return nullptr;
            }
            return parsed_chunks[fourcc];
        }

        bool has_chunk(uint32_t fourcc) { return chunks.find(fourcc) != chunks.end(); }

        bool has_chunk_impl(uint32_t fourcc) { return parsed_chunks.find(fourcc) != parsed_chunks.end(); }
    };
} // namespace dragon
