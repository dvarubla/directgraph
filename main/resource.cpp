#include "resource.h"
#include <common.h>
#include <stdio.h>

namespace directgraph{
    void load_resource(uint_fast32_t name, uint_fast32_t type, uint_fast32_t &size, void *&data) {
        HMODULE handle = HINST_THISCOMPONENT;
        HRSRC rc = FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
        HGLOBAL rcData = LoadResource(handle, rc);
        size = SizeofResource(handle, rc);
        data = LockResource(rcData);
    }
}