//
// Created by yretenai on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"
#include "WemDataIndex.hpp"

namespace dragon::bkhd {
    class WemData : public WemChunk {
      public:
        std::shared_ptr<dragon::Array<uint8_t>> data;

        WemData(dragon::Array<uint8_t> buffer) {
            data = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size(), nullptr);
        }

        dragon::Array<uint8_t> get_stream(WemDataIndex::DataIndexEntry entry) {
            return dragon::Array<uint8_t>(data, entry.offset, entry.size);
        }

        dragon::Array<uint8_t> get_stream(WemDataIndex* index, uint32_t id) {
            return get_stream(index->streams[id]);
        }
    };
}
