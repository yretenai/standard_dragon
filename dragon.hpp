//
// Created by yretenai on 5/28/2020.
//

#pragma once

#define DRAGON_VERSION_I 6
#define DRAGON_VERSION_MAJOR 1
#define DRAGON_VERSION_MINOR 3
#define DRAGON_VERSION_PATCH 0
#define DRAGON_VERSION "1.2.0"

#include "Array.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

#define DRAGON_MAGIC32(ch0, ch1, ch2, ch3) \
    ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) | ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24))

#define DRAGON_MAGIC64(ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7) \
    ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) | ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24) | \
     ((uint32_t)(uint8_t)(ch4) << 32) | ((uint32_t)(uint8_t)(ch5) << 40) | ((uint32_t)(uint8_t)(ch6) << 48) | ((uint32_t)(uint8_t)(ch7) << 56))

#define DRAGON_ASSERT static_assert

#ifndef DRAGON_LIBRARY_NAME
#define DRAGON_LIBRARY_NAME "dragon"
#endif

#define DRAGON_MAX_PATH 32767

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifndef NDEBUG
#ifdef _WIN32
#define DRAGON_BREAK __debugbreak()
#else
#include <csignal>
#define DRAGON_BREAK raise(SIGTRAP)
#endif
#else
#define DRAGON_BREAK
#endif

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define HEXLOG64 std::setfill('0') << std::hex << std::setw(16)
#define HEXLOG32 std::setfill('0') << std::hex << std::setw(8)
#define HEXLOG16 std::setfill('0') << std::hex << std::setw(4)
#define BITLOG8(value) std::bitset<8>(value)
#define BITLOG16(value) std::bitset<16>(value)
#define BITLOG32(value) std::bitset<32>(value)
#define BITLOG64(value) std::bitset<64>(value)

#ifndef DRAGON_OLOG
#define DRAGON_OLOG(out, msg) (out << msg << std::endl << std::flush)
#endif

#define DRAGON_LOG(msg) DRAGON_OLOG(std::cout, "[o][" << DRAGON_LIBRARY_NAME << "][" << __PRETTY_FUNCTION__ << "] " << msg)
#define DRAGON_ELOG(msg) DRAGON_OLOG(std::cerr, "[e][" << DRAGON_LIBRARY_NAME << "][" << __PRETTY_FUNCTION__ << "] " << msg)
#define DRAGON_EPRINTF(...) fprintf(stderr, __VA_ARGS__)

namespace dragon {
    [[maybe_unused]] inline int Align(int value, int align) {
        int v = value % align;
        if (v != 0)
            return value + align - v;
        return value;
    }

    [[maybe_unused]] inline Array<uint8_t> read_file(const std::filesystem::path& path) {
#ifdef DRAGON_TOOLS
        DRAGON_LOG("Reading file " << path);
#endif
        std::ifstream file(path, std::ios::binary | std::ios::in);
        auto size = (size_t)std::filesystem::file_size(path);
        Array<uint8_t> bytes(size, nullptr);
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(bytes.data()), size);
        file.close();
        return bytes;
    }

    [[maybe_unused]] inline void write_file(const std::filesystem::path& path, const Array<uint8_t>& buffer) {
        if (buffer.empty())
            return;
#ifdef DRAGON_TOOLS
        if (std::filesystem::exists(path)) {
            DRAGON_ELOG("Overwriting file " << path);
        } else {
            DRAGON_LOG("Writing file " << path);
        }
#endif
        std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
        file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        file.flush();
        file.close();
    }
} // namespace dragon
