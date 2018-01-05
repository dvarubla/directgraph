#pragma once

#include <stdint.h>

namespace directgraph {
    void load_resource(uint_fast32_t name, uint_fast32_t type, uint_fast32_t &size, void*& data);
}

