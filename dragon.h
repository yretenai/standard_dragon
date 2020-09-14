//
// Created by yretenai on 5/28/2020.
//

#pragma once

#define DRAGON_VERSION_I 4
#define DRAGON_VERSION_MAJOR 1
#define DRAGON_VERSION_MINOR 1
#define DRAGON_VERSION_PATCH 0
#define DRAGON_VERSION "1.1.0"

#include "Array.hpp"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
    ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) | ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24))
#endif // MAKEFOURCC

#ifndef FOURCC_DX10
#define FOURCC_DX10 (MAKEFOURCC('D', 'X', '1', '0'))
#endif

#ifndef DRAGON_LIBRARY_NAME
#define DRAGON_LIBRARY_NAME "dragon"
#endif

#ifndef MAX_PATH_NAME_LEN
#define MAX_PATH_NAME_LEN 32767
#endif

#ifdef _WIN32
#ifdef USE_NOESIS

#include "../noesis/pluginshare.h"

#else // USE_NOESIS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#endif // USE_NOESIS
#endif

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define HEXLOG64 std::setfill('0') << std::hex << std::setw(16)
#define HEXLOG32 std::setfill('0') << std::hex << std::setw(8)
#define HEXLOG16 std::setfill('0') << std::hex << std::setw(4)

#ifdef USE_NOESIS
#define OLOG(out, msg)                                                    \
    do {                                                                  \
        std::stringstream s;                                              \
        s << msg << std::endl;                                            \
        if (g_nfn != nullptr && g_nfn->NPAPI_DebugLogIsOpen())            \
            g_nfn->NPAPI_DebugLogStr(const_cast<char*>(s.str().c_str())); \
        (out << s.str() << std::flush);                                   \
    } while (0)
#else
#define OLOG(out, msg) (out << msg << std::endl << std::flush)
#endif

#define LOG(msg) OLOG(std::cout, "[o][" << DRAGON_LIBRARY_NAME << "][" << __PRETTY_FUNCTION__ << "] " << msg)
#define ELOG(msg) OLOG(std::cerr, "[e][" << DRAGON_LIBRARY_NAME << "][" << __PRETTY_FUNCTION__ << "] " << msg)

#ifndef eprintf
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#else
#error eprintf already defined?
#endif

namespace dragon {
    inline int Align(int value, int align) {
        int v = value % align;
        if (v != 0)
            return value + align - v;
        return value;
    }

    inline Array<uint8_t> read_file(const std::filesystem::path& path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);
        size_t size = (size_t)std::filesystem::file_size(path);
        Array<uint8_t> bytes(size, nullptr);
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(bytes.data()), size);
        file.close();
        return bytes;
    }

    inline void write_file(const std::filesystem::path& path, const Array<uint8_t>& buffer) {
        if (buffer.empty())
            return;
        std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
        file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        file.flush();
        file.close();
    }
} // namespace dragon
