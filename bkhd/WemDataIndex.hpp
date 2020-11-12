//
// Created by yretenai on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"
#include <map>

namespace dragon::bkhd {
    class WemDataIndex : public WemChunk {
      public:
#pragma pack(push, 1)
        typedef struct BNK_DIDX_ENTRY {
            uint32_t stream_id = 0;
            uint32_t offset = 0;
            uint32_t size = 0;
        } DataIndexEntry;
        DRAGON_ASSERT(sizeof(DataIndexEntry) == 12, "Bnk DIDX Entry has an invalid size");
#pragma pack(pop)
        std::map<uint32_t, DataIndexEntry> streams;

        WemDataIndex(dragon::Array<uint8_t> buffer) {
            dragon::Array<DataIndexEntry> didx = buffer.cast<DataIndexEntry>(0, buffer.size() / sizeof(DataIndexEntry));
            for (DataIndexEntry entry : didx) {
                streams[entry.stream_id] =  entry;
            }
        }

        bool has_stream(uint32_t stream_id) {
            return streams.find(stream_id) != streams.end();
        }
    };
}
