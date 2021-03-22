//
// Created by Yretenai on 11/12/2020.
//

#pragma once
#ifndef DRAGON_INDENT_HPP
#define DRAGON_INDENT_HPP

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

        explicit Indent(size_t l) : level(l > 0 ? l : 0), text(level * 2, ' ') {}

        Indent operator+(const uint64_t &v) const {
            return Indent(level + v);
        }

        Indent operator-(const uint64_t &v) const {
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

        Indent& operator=(const uint64_t &v) {
            level = v;
            text = std::string(level * 2, ' ');
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, const Indent &indent) {
            os << indent.text;
            return os;
        }
    };
}

#endif // DRAGON_INDENT_HPP
