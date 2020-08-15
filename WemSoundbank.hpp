//
// Created by yretenai on 2020-08-16.
//

#ifndef DRAGON_WEMSOUNDBANK_H
#define DRAGON_WEMSOUNDBANK_H

#include "Array.hpp"
#include <memory>
#include <map>

namespace Dragon {
    class WemSoundbank {
      public:
        WemSoundbank(dragon::Array<char> buffer) {

        }

        ~WemSoundbank() = default;

        std::map<uint32_t, std::shared_ptr<dragon::Array<char>>> Streams;
    };
}

#endif // DRAGON_WEMSOUNDBANK_H
