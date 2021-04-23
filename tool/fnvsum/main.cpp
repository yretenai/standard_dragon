//
// Created by Lilith on 2020-10-04.
//

#include <standard_dragon/dragon.hpp>
#include <standard_dragon/hash/fnv1a.hpp>

#ifdef DRAGON_FNVMODE_32
#define DRAGON_FNV dragon::hash::fnv1_32
#define DRAGON_FNV_S "fnv1"
#endif
#ifdef DRAGON_FNVMODE_64
#define DRAGON_FNV dragon::hash::fnv1_64
#define DRAGON_FNV_S "fnv1"
#endif
#ifdef DRAGON_FNVMODE_A32
#define DRAGON_FNV dragon::hash::fnv1a32
#define DRAGON_FNV_S "fnv1a"
#endif
#ifdef DRAGON_FNVMODE_A64
#define DRAGON_FNV dragon::hash::fnv1a64
#define DRAGON_FNV_S "fnv1a"
#endif
#ifndef DRAGON_FNV
#define DRAGON_FNV dragon::hash::fnv1_32
#define DRAGON_FNV_S "32-bit fnv1"
#endif
#ifndef DRAGON_FNVBITS
#define DRAGON_FNVBITS 64
#endif

int main(int argc, char** argv) {

    if (argc < 2) {
        DRAGON_ELOG("Flavor: " << DRAGON_FNVBITS << "-bit " << DRAGON_FNV_S);
        DRAGON_ELOG("USAGE: " << argv[0] << " strings...");
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        auto hash = DRAGON_FNV(reinterpret_cast<const uint8_t*>(argv[i]), strlen(argv[i]), DRAGON_FNVBITS);
        std::cout << std::hex << hash << "\t" << std::dec << hash << "\t" << argv[i] << std::endl;
    }
}
