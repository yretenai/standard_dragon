//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"

namespace dragon::bkhd::hirc {
    class WemHierarchyUnimplemented : public WemHierarchyChunk {
    public:
        explicit WemHierarchyUnimplemented(dragon::Array<uint8_t> &buffer, uintptr_t &offset, uint32_t &version) {
            data = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size());
        };

        std::shared_ptr<dragon::Array<uint8_t>> data;
    };
} // namespace dragon::bkhd::hirc
