//
// Created by Lilith on 2021-06-06.
//

#pragma once

#include <exception>
#include <string>
#include <utility>

namespace dragon::exception {
    class out_of_bounds : public std::exception {
    public:
        std::string reason_message;
        explicit out_of_bounds(const char *message = "out of bounds") { reason_message = message; }
        explicit out_of_bounds(std::string message) { reason_message = std::move(message); }

        const char *what() const noexcept override { return reason_message.c_str(); }
    };
} // namespace dragon::exception
