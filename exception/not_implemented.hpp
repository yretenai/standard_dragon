//
// Created by Lilith on 2020-09-09.
//

#pragma once

#include <exception>
#include <string>

namespace dragon::exception {
    class not_implemented : public std::exception {
      public:
        std::string reason_message;
        explicit not_implemented(const char* message = "not implemented") { reason_message = message; }

        const char* what() const noexcept override { return reason_message.c_str(); }
    };
} // namespace dragon::exception
