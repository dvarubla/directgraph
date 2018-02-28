#include "resource.h"
#include "WException.h"
#include <common.h>

namespace directgraph{
    void load_resource(uint_fast32_t name, uint_fast32_t type, uint_fast32_t &size, void *&data) {
        HMODULE handle = HINST_THISCOMPONENT;
        HRSRC rc = FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
        if(rc == NULL){
            THROW_EXC_CODE(WException, CANT_CREATE_RESOURCE, L"Can't find resource");
        }
        HGLOBAL rcData = LoadResource(handle, rc);
        if(rcData == NULL){
            THROW_EXC_CODE(WException, CANT_CREATE_RESOURCE, L"Can't load resource");
        }
        size = SizeofResource(handle, rc);
        data = LockResource(rcData);
    }
}