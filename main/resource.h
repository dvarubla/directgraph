#ifndef DIRECTGRAPH_RESOURCE_H
#define DIRECTGRAPH_RESOURCE_H

#include <stdint.h>

namespace directgraph {
    void load_resource(uint_fast32_t name, uint_fast32_t type, uint_fast32_t &size, void*& data);
}

#endif //DIRECTGRAPH_RESOURCE_H
