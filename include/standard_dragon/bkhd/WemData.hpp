//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"
#include "WemDataIndex.hpp"

namespace dragon::bkhd {
    class WemData : public WemChunk {
    public:
        const static uint32_t fourcc = DRAGON_MAGIC32('D', 'A', 'T', 'A');

        std::shared_ptr<dragon::Array<uint8_t>> data;

        explicit WemData(dragon::Array<uint8_t> &buffer) {
            data = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size());
        }

        [[nodiscard]] dragon::Array<uint8_t> get_stream(WemDataIndex::DataIndexEntry entry) const {
            return dragon::Array<uint8_t>(data, entry.offset, entry.size);
        }

        [[nodiscard]] dragon::Array<uint8_t> get_stream(WemDataIndex *index, uint32_t id) const { return get_stream(index->streams[id]); }
    };
} // namespace dragon::bkhd
