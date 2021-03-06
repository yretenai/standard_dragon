//
// Created by yretenai on 2020-09-08.
//

#pragma once

#include <exception>
#include <string>

namespace dragon::exception {
    class invalid_data : public std::exception {
      public:
        std::string reason_message;
        explicit invalid_data(const char* message = "invalid or unexpected data") { reason_message = message; }

        const char* what() const noexcept override { return reason_message.c_str(); }
    };
} // namespace dragon::exception
