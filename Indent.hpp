//
// Created by Yretenai on 11/12/2020.
//

#pragma once

#include <cstdint>
#include <string>
#include <istream>
#include <ostream>

namespace dragon {
    class Indent {
    private:
        uint64_t level = 0;
    public:
        std::string text;

        Indent(size_t l) : level(l > 0 ? l : 0), text(level * 2, ' ') {}

        Indent operator+(const uint64_t &v) {
            return Indent(level + v);
        }

        Indent operator-(const uint64_t &v) {
            return Indent(level - v);
        }

        Indent operator+=(const uint64_t &v) {
            level += v;
            text = std::string(level * 2, ' ');
            return *this;
        }

        Indent operator-=(const uint64_t &v) {
            if (v > level) level = 0;
            else level -= v;
            text = std::string(level * 2, ' ');
            return *this;
        }

        Indent operator=(const uint64_t &v) {
            level = v;
            text = std::string(level * 2, ' ');
            return *this;
        }
    };

    std::ostream &operator<<(std::ostream &os, const Indent &indent) {
        os << indent.text;
        return os;
    }
}
