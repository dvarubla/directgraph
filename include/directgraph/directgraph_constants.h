#ifndef DIRECTGRAPH_GRAPHICS_CONST_H
#define DIRECTGRAPH_GRAPHICS_CONST_H

#include "directgraph_stdint.h"

typedef enum DirectgraphEvent{
    DIRECTGRAPH_WND_CREATED = 60000,
    DIRECTGRAPH_WND_CREATE,
    DIRECTGRAPH_WND_DESTROY,
    DIRECTGRAPH_WND_DESTROYED,
    DIRECTGRAPH_REPLY
} DirectgraphEvent;

typedef enum DirectgraphRendererType{
    DIRECTGRAPH_DX9_RENDERER = 0
} DirectgraphRendererType;

typedef enum DirectgraphControllerType{
    DIRECTGRAPH_MULT_THREAD_CTRL = 0
} DirectgraphControllerType;

typedef enum DirectgraphErrorCode{
    DIRECTGRAPH_UNREACHABLE_CODE = 0,
    DIRECTGRAPH_CANT_ALLOC,
    DIRECTGRAPH_CANT_CREATE_WINDOW,
    DIRECTGRAPH_NO_WINDOWS,
    DIRECTGRAPH_WRONG_WINDOW_INDEX,

    DIRECTGRAPH_CANT_CREATE_DIRECTX9 = 2048,
    DIRECTGRAPH_DX9_CANT_GET_DISPLAY_MODE,
    DIRECTGRAPH_DX9_CANT_CREATE_DEVICE,
    DIRECTGRAPH_DX9_CANT_CREATE_EXTRA_WIN,
    DIRECTGRAPH_DX9_CANT_SET_RENDER_STATE,
    DIRECTGRAPH_DX9_CANT_CREATE_TEXTURE,
    DIRECTGRAPH_DX9_CANT_CREATE_VBUFFER,
    DIRECTGRAPH_DX9_UNSUPPORTED_DISPLAY_FORMAT,
    DIRECTGRAPH_DX9_NO_TEXTURE_FORMAT
} DirectgraphErrorCode;

typedef uint32_t DirectgraphWinIndex;

typedef enum fill_patterns {
    EMPTY_FILL,      // использовать цвет фона
    SOLID_FILL,      // использовать цвет закраски
    LINE_FILL,       // ---
    LTSLASH_FILL,    // ///
    SLASH_FILL,      // /// толстые
    BKSLASH_FILL,    // "\\\" толстые
    LTBKSLASH_FILL,  // "\\\"
    HATCH_FILL,      // light hatch fill
    XHATCH_FILL,     // Heavy cross hatch
    INTERLEAVE_FILL, // Interleaving line
    WIDE_DOT_FILL,   // Widely spaced dot
    CLOSE_DOT_FILL,  // Closely spaced dot
    USER_FILL        // User-defined fill
} fill_patterns;

typedef void (*DirectgraphErrorHandler)(
        const wchar_t *filename,
        uint32_t lineNum,
        DirectgraphErrorCode code,
        const wchar_t *str
);

#endif //DIRECTGRAPH_GRAPHICS_CONST_H
