#ifndef DIRECTGRAPH_GRAPHICS_CONST_H
#define DIRECTGRAPH_GRAPHICS_CONST_H

typedef enum DirectGraphEvent{
    DIRECTGRAPH_WND_CREATED = 60000,
    DIRECTGRAPH_WND_CREATE,
    DIRECTGRAPH_WND_QUIT,
    DIRECTGRAPH_REPLY
} DirectGraphEvent;

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

#endif //DIRECTGRAPH_GRAPHICS_CONST_H
