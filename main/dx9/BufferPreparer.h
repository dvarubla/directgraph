#pragma once

#include <stdint.h>
#include <main/IQueueReader.h>
#include <vector>
#include "PrimitiveCreator.h"

namespace directgraph {
    namespace dx9 {
        class BufferPreparer {
        public:
            enum DrawOpType{
                CLEAR,
                SET_FILL_PATTERN,
                SET_USER_FILL_PATTERN,
                SET_BG_COLOR,
                ITEMS,
                SET_PIXEL_TEXTURE
            };
            enum DrawDataType{
                RECT_VERTEX,
                TEXTURED_RECT_VERTEX,
                TEXTURED_VERTEX
            };
            struct DrawOp{
                DrawOpType type;
                union {
                    uint8_t fillPattern;
                    uint32_t bgColor;
                    char *userFillPattern;
                    struct Items{
                        uint32_t offset;
                        uint32_t numItems;
                        DrawDataType type;
                    } items;
                    IPixelContainer *pixelContainer;
                } data;
            };
            struct DevDrawState{
                uint_fast8_t fillPattern;
                uint_fast32_t bgColor;
                char *userFillPattern;
            };

            struct GenDataVars{
                uint_fast32_t fillColor;
                uint_fast32_t bgColor;
                uint_fast8_t fillStyle;
            };
            typedef std::vector<DrawOp> DrawOpVector;
            typedef std::vector<char*> CharPVector;

            const static int VERTICES_TRIANGLES_DIFF = 2;
        private:
            const static int TRIANGLES_IN_QUAD = 2;
            const static int VERTICES_IN_QUAD = 4;

            PrimitiveCreator _primCreator;
            void *_vertMem;
            uint_fast32_t _memSize;
            const DevDrawState *_state;
            DevDrawState _lastState;
            DrawOpVector _drawOps;
            CharPVector _patterns;
            DPIHelper *_helper;
            uint_fast32_t _pixelTextureWidth;
            uint_fast32_t _pixelTextureHeight;
            GenDataVars _curGenDataVars;
            uint_fast32_t _curUsedSize;
        public:
            BufferPreparer(
                    uint_fast32_t memSize, const DevDrawState *state, DPIHelper *_helper,
                    uint_fast32_t pxTextureWidth, uint_fast32_t pxTextureHeight,
                    const GenDataVars &vars
            );
            ~BufferPreparer();
            void prepareBuffer(IQueueReader *reader);
            void *getBuffer();
            void clear();
            DrawOpVector::iterator drawOpsBegin();
            DrawOpVector::iterator drawOpsEnd();
            bool isEmpty();
            uint_fast32_t getUsedSize();
        };
    }
}


