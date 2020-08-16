//
// Created by yretenai on 2020-08-16.
//

#ifndef DRAGON_WEMSOUNDBANK_H
#define DRAGON_WEMSOUNDBANK_H

#include "Array.hpp"
#include <map>
#include <memory>

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
        const uint32_t DIDX_FOURCC = MAKEFOURCC('D', 'I', 'D', 'X');
        const uint32_t DATA_FOURCC = MAKEFOURCC('D', 'A', 'T', 'A');
        WemSoundbank(dragon::Array<char> buffer) {
            BaseStream = std::make_shared<dragon::Array<char>>(buffer.data(), buffer.byte_size(), nullptr);
            uintptr_t ptr = 0;
            while (ptr < BaseStream->size()) {
                BnkChunkHeader header = BaseStream->lpcast<BnkChunkHeader>(&ptr);
                Chunks[header.fourcc] = std::make_shared<dragon::Array<char>>(BaseStream, ptr, header.size);
            }
            if (Chunks.find(DIDX_FOURCC) != Chunks.end() && Chunks.find(DATA_FOURCC) != Chunks.end()) {
                std::shared_ptr<dragon::Array<char>> didx_stream = Chunks[DIDX_FOURCC];
                std::shared_ptr<dragon::Array<char>> data_stream = Chunks[DATA_FOURCC];
                dragon::Array<DataIndexEntry> didx = didx_stream->cast<DataIndexEntry>(0, didx_stream->size() / sizeof(DataIndexEntry));
                for (DataIndexEntry entry : didx) {
                    Streams[entry.stream_id] = std::make_shared<dragon::Array<char>>(data_stream, entry.offset, entry.length);
                }
            }
        }

        ~WemSoundbank() = default;
        std::shared_ptr<dragon::Array<char>> BaseStream;
        std::map<uint32_t, std::shared_ptr<dragon::Array<char>>> Chunks;
        std::map<uint32_t, std::shared_ptr<dragon::Array<char>>> Streams;
    };
} // namespace dragon

#endif // DRAGON_WEMSOUNDBANK_H
