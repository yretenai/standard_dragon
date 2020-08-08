//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef DRAGON_DRAGON_H
#define DRAGON_DRAGON_H

#define DRAGON_VERSION_I 3
#define DRAGON_VERSION_MAJOR 1
#define DRAGON_VERSION_MINOR 0
#define DRAGON_VERSION_PATCH 3
#define DRAGON_VERSION "1.0.3"

#include "Array.h"
#include <assert.h>
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

extern std::ofstream* LogStream;

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

#ifdef USE_NOESIS
#define LOG(msg)                                                                                    \
    do {                                                                                            \
        std::stringstream s;                                                                        \
        s << "[" << DRAGON_LIBRARY_NAME << "][" << __PRETTY_FUNCTION__ << "] " << msg << std::endl; \
        if (LogStream != nullptr)                                                                   \
            (*LogStream << s.str() << std::flush);                                                  \
        if (g_nfn != nullptr && g_nfn->NPAPI_DebugLogIsOpen())                                      \
            g_nfn->NPAPI_DebugLogStr(const_cast<char*>(s.str().c_str()));                           \
        (std::cout << s.str() << std::flush);                                                       \
    } while (0)
#else
#define LOG(msg)                                                                                                                              \
    ((LogStream != nullptr ? *LogStream : std::cout) << "[" << DRAGON_LIBRARY_NAME << "][" << __PRETTY_FUNCTION__ << "] " << msg << std::endl \
                                                     << std::flush)
#endif

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

    inline Array<char> read_file(std::filesystem::path path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);
        auto size = (size_t)std::filesystem::file_size(path);
        Array<char> bytes(size, nullptr);
        file.seekg(0, std::ios::beg);
        file.read(bytes.data(), size);
        return bytes;
    }

    inline void write_file(std::filesystem::path path, Array<char>* buffer) {
        if (buffer->empty())
            return;
        std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
        file.write(reinterpret_cast<const char*>(buffer->data()), buffer->size());
    }

    class not_implemented_exception : public std::exception {};
} // namespace dragon

#endif // DRAGON_DRAGON_H
