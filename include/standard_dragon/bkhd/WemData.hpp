//
// Created by Lilith on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"
#include "WemDataIndex.hpp"

namespace dragon::bkhd {
    class WemData : public WemChunk {
      public:
        std::shared_ptr<dragon::Array<uint8_t>> data;

        explicit WemData(const dragon::Array<uint8_t>& buffer) {
            data = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size(), true);
        }

        [[nodiscard]] dragon::Array<uint8_t> get_stream(WemDataIndex::DataIndexEntry entry) const {
            return dragon::Array<uint8_t>(data, entry.offset, entry.size);
        }

        [[nodiscard]] dragon::Array<uint8_t> get_stream(WemDataIndex* index, uint32_t id) const {
            return get_stream(index->streams[id]);
        }
    };
}
