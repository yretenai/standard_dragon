//
// Created by yretenai on 2020-08-30.
//

#pragma once

#include "WemChunk.hpp"

namespace dragon::bkhd {
    class WemSoundTypeGroup : public WemChunk {
      public:
        WemSoundTypeGroup(dragon::Array<uint8_t> buffer) {}
    };
} // namespace dragon::bkhd
