//
// Created by Lilith on 2020-09-08.
//

#pragma once

#include <standard_dragon/exception/reasonable.hpp>

namespace dragon::exception {
    class invalid_data : public reasonable {
    public:
        explicit invalid_data(const char *message = "unexpected error") : reasonable(message) { }
    };
} // namespace dragon::exception
