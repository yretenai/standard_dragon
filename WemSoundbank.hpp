//
// Created by yretenai on 2020-08-16.
//

#ifndef DRAGON_WEMSOUNDBANK_H
#define DRAGON_WEMSOUNDBANK_H

#include "Array.hpp"
#include <map>
#include <memory>
#include <cstdbool>
#include "dragon.h"

namespace dragon {
    class WemSoundbank {
      public:
#pragma pack(push, 1)
        typedef struct BNK_CHUNK_HEADER {
            uint32_t fourcc = 0;
            uint32_t size = 0;
        } BnkChunkHeader;
        typedef struct BNK_DIDX_ENTRY {
            uint32_t stream_id = 0;
            uint32_t offset = 0;
            uint32_t size = 0;
        } DataIndexEntry;
#pragma pack(pop)
        const uint32_t BKHD_FOURCC = MAKEFOURCC('B', 'K', 'H', 'D');
        const uint32_t DIDX_FOURCC = MAKEFOURCC('D', 'I', 'D', 'X');
        const uint32_t DATA_FOURCC = MAKEFOURCC('D', 'A', 'T', 'A');
        WemSoundbank(dragon::Array<char> buffer) {
            base_stream = std::make_shared<dragon::Array<char>>(buffer.data(), buffer.byte_size(), nullptr);
            uintptr_t ptr = 0;
            while (ptr < base_stream->size()) {
                BnkChunkHeader header = base_stream->lpcast<BnkChunkHeader>(&ptr);
                chunks[header.fourcc] = std::pair<uintptr_t, BnkChunkHeader>(ptr, header);
            }
            if (has_chunk(BKHD_FOURCC)) {
                dragon::Array<char> bkhd_stream = get_chunk(BKHD_FOURCC);
                version = bkhd_stream.cast<uint32_t>(0);
                id = bkhd_stream.cast<uint32_t>(4);
            }
            if (has_chunk(DIDX_FOURCC) && has_chunk(DATA_FOURCC)) {
                dragon::Array<char> didx_stream = get_chunk(DIDX_FOURCC);
                dragon::Array<DataIndexEntry> didx = didx_stream.cast<DataIndexEntry>(0, didx_stream.size() / sizeof(DataIndexEntry));
                for (DataIndexEntry entry : didx) {
                    streams[entry.stream_id] =  entry;
                }
            }
        }

        ~WemSoundbank() = default;

        std::shared_ptr<dragon::Array<char>> base_stream;
        std::map<uint32_t, std::pair<uintptr_t, BnkChunkHeader>> chunks;
        std::map<uint32_t, DataIndexEntry> streams;
        uint32_t version;
        uint32_t id;

        dragon::Array<char> get_chunk(uint32_t fourcc) {
            if(!has_chunk(fourcc)) {
                return dragon::Array<char>();
            }

            std::pair<uintptr_t, BnkChunkHeader> pair = chunks[fourcc];
            return dragon::Array<char>(base_stream, pair.first, pair.second.size);
        }

        bool has_chunk(uint32_t fourcc) {
            return chunks.find(fourcc) != chunks.end();
        }

        dragon::Array<char> get_stream(uint32_t stream_id) {
            if(!has_stream(id) || !has_chunk(DATA_FOURCC)) {
                return dragon::Array<char>();
            }

            std::pair<uintptr_t, BnkChunkHeader> pair = chunks[DATA_FOURCC];
            DataIndexEntry entry = streams[stream_id];
            return dragon::Array<char>(base_stream, pair.first + entry.offset, entry.size);
        }

        bool has_stream(uint32_t stream_id) {
            return streams.find(stream_id) != streams.end();
        }
    };
} // namespace dragon

#endif // DRAGON_WEMSOUNDBANK_H
