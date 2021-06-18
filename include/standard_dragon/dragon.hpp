//
// Created by Lilith on 2020-05-28.
//

#pragma once

#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <type_traits>

#include "Array.hpp"
#include "macros.hpp"

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

    inline void str_to_lower(std::string &str) {
        std::transform(str.begin(), str.end(), str.begin(), [](char c) { return std::tolower(c); });
    }

    inline void str_to_upper(std::string &str) {
        std::transform(str.begin(), str.end(), str.begin(), [](char c) { return std::toupper(c); });
    }

    inline void find_paths(const std::filesystem::path &path, std::deque<std::filesystem::path> &container, const std::set<std::string> &filters = {}, std::filesystem::directory_options options = {}) {
        if (!std::filesystem::exists(path)) {
            return;
        }

        if (std::filesystem::is_directory(path)) {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(path, options)) {
                if (entry.is_directory()) {
                    continue;
                }

                const auto &sub_path = entry.path();
                auto ext             = sub_path.extension();
                if (!filters.empty()) {
                    bool found = false;
                    for (const auto &filter : filters) {
                        if (ext == filter) {
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        continue;
                    }
                }
                container.emplace_back(sub_path);
            }
        } else {
            container.emplace_back(path);
        }
    }

    inline std::deque<std::filesystem::path> find_paths(std::deque<std::string> &paths, const std::set<std::string> &filters = {}, std::filesystem::directory_options options = {}) {
        std::deque<std::filesystem::path> container;

        for (const auto &path : paths) {
            find_paths(path, container, filters, options);
        }

        return container;
    }

    inline std::deque<std::filesystem::path> find_paths(std::deque<std::filesystem::path> &paths, const std::set<std::string> &filters = {}, std::filesystem::directory_options options = {}) {
        std::deque<std::filesystem::path> container;

        for (const auto &path : paths) {
            find_paths(path, container, filters, options);
        }

        return container;
    }
} // namespace dragon
