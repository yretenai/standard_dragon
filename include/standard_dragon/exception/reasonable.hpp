//
// Created by Lilith on 2020-09-08.
//

#pragma once

#include <exception>
#include <string>
#include <utility>

namespace dragon::exception {
    class reasonable : public std::exception {
    public:
        std::string reason_message;
        explicit reasonable(const char *message = "unexpected error") : std::exception() { reason_message = message; }
        explicit reasonable(std::string message) : std::exception() { reason_message = std::move(message); }

        [[nodiscard]] const char *what() const noexcept override { return reason_message.c_str(); }
    };
} // namespace dragon::exception
