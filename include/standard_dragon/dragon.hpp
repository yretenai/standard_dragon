//
// Created by Lilith on 2020-05-28.
//

#pragma once

#include "macros.hpp"
#include "Array.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <type_traits>

#ifdef _WIN32
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif // WIN32_LEAN_AND_MEAN
#    include <Windows.h>
#endif

namespace dragon {
    template<typename T>
    inline typename std::enable_if<std::is_integral<T>::value, T>::type Align(T value, T align) {
        T v = value % align;
        if (v != 0)
            return value + align - v;
        return value;
    }

    inline Array<uint8_t> read_file(const std::filesystem::path &path) {
#ifdef DRAGON_TOOLS
        DRAGON_LOG("Reading file " << path);
#endif
        std::ifstream file(path, std::ios::binary | std::ios::in);
        auto size = (size_t) std::filesystem::file_size(path);
        Array<uint8_t> bytes(size, nullptr);
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char *>(bytes.data()), (std::streamsize) size);
        file.close();
        return bytes;
    }

    inline void write_file(const std::filesystem::path &path, const Array<uint8_t> &buffer) {
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
        file.write(reinterpret_cast<const char *>(buffer.data()), (std::streamsize) buffer.size());
        file.flush();
        file.close();
    }
} // namespace dragon
