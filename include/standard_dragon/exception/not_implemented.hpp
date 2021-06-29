//
// Created by Lilith on 2020-09-09.
//

#pragma once

#include <standard_dragon/exception/reasonable.hpp>

namespace dragon::exception {
    class not_implemented : public reasonable {
    public:
        explicit not_implemented(const char *message = "not implemented") : reasonable(message) { }
    };
} // namespace dragon::exception
