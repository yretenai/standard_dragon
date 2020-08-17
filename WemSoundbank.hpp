//
// Created by yretenai on 2020-08-16.
//

#ifndef DRAGON_WEMSOUNDBANK_H
#define DRAGON_WEMSOUNDBANK_H

#include "Array.hpp"
#include <map>
#include <memory>
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
            uint32_t length = 0;
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
                chunks[header.fourcc] = std::make_shared<dragon::Array<char>>(base_stream, ptr, header.size);
            }
            if (chunks.find(BKHD_FOURCC) != chunks.end()) {
                std::shared_ptr<dragon::Array<char>> bkhd_stream = chunks[BKHD_FOURCC];
                version = bkhd_stream->cast<uint32_t>(0);
                id = bkhd_stream->cast<uint32_t>(4);
            }
            if (chunks.find(DIDX_FOURCC) != chunks.end() && chunks.find(DATA_FOURCC) != chunks.end()) {
                std::shared_ptr<dragon::Array<char>> didx_stream = chunks[DIDX_FOURCC];
                std::shared_ptr<dragon::Array<char>> data_stream = chunks[DATA_FOURCC];
                dragon::Array<DataIndexEntry> didx = didx_stream->cast<DataIndexEntry>(0, didx_stream->size() / sizeof(DataIndexEntry));
                for (DataIndexEntry entry : didx) {
                    streams[entry.stream_id] = std::make_shared<dragon::Array<char>>(data_stream, entry.offset, entry.length);
                }
            }
        }

        ~WemSoundbank() = default;
        std::shared_ptr<dragon::Array<char>> base_stream;
        std::map<uint32_t, std::shared_ptr<dragon::Array<char>>> chunks;
        std::map<uint32_t, std::shared_ptr<dragon::Array<char>>> streams;
        uint32_t version;
        uint32_t id;
    };
} // namespace dragon

#endif // DRAGON_WEMSOUNDBANK_H
