//
// Created by yretenai on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemSoundTypeId : public WemChunk {
      public:
        WemSoundTypeId(dragon::Array<uint8_t> buffer) {}
    };
} // namespace dragon::bkhd
