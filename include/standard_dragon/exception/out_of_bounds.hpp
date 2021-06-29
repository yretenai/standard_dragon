//
// Created by Lilith on 2021-06-06.
//

#pragma once

#include <standard_dragon/exception/reasonable.hpp>

namespace dragon::exception {
    class out_of_bounds : public reasonable {
    public:
        explicit out_of_bounds(const char *message = "out of bounds") : reasonable(message) { }
    };
} // namespace dragon::exception
