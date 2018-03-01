#pragma once

#include <stdint.h>
#include <main/IQueueReader.h>
#include <vector>
#include "PrimitiveCreator.h"
#include "ShaderManager.h"

namespace directgraph {
    namespace dx9 {
        class BufferPreparer {
        public:
            enum DrawOpType{
                CLEAR,
                REMOVE_TEXTURE,
                SET_FILL_PATTERN,
                SET_USER_FILL_PATTERN,
                SET_BG_COLOR,
                ITEMS,
                SET_PIXEL_TEXTURE,
                SET_LINE_STYLE
            };
            enum DrawDataType{
                RECT_VERTEX,
                TEXTURED_RECT_VERTEX,
                TEXTURED_VERTEX,
                ELLIPSE_VERTEX
            };
            enum ShaderType{
                NO_SHADER,
                ELLIPSE_SHADER
            };
            enum TextureState{
                NO_TEXTURE,
                FILL_TEXTURE,
                LINE_TEXTURE
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
                    uint8_t lineStyle;
                    IPixelContainer *pixelContainer;
                } data;
            };
            struct DevDrawState{
                TextureState textureState;
                uint_fast8_t fillPattern;
                uint_fast8_t lineStyle;
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
            ShaderManager *_shaderMan;
            void *_vertMem;
            uint_fast32_t _memSize;
            DevDrawState _lastState, _curState;
            DrawOpVector _drawOps;
            CharPVector _patterns;
            DPIHelper *_helper;
            uint_fast32_t _width;
            uint_fast32_t _height;
            uint_fast32_t _pixelTextureWidth;
            uint_fast32_t _pixelTextureHeight;
            GenDataVars _curGenDataVars;
            ShaderType _shaderType;
            uint_fast32_t _curUsedSize;
            uint_fast32_t _lastOffset;
            bool _canReadMore;
            void processStateChanges(const QueueItem &item, bool &isFirst);
            struct TypeSize{
                uint_fast32_t sizeMult;
                DrawDataType drawDataType;
            };
            TypeSize getTypeSize(const QueueItem &item);
            void processDrawItem(
                    const QueueItem &item, bool &isFirst, void *&curVertMem, int_fast32_t &prevX, int_fast32_t &prevY
            );
            void useFillTexture(bool &isFirst);
            void disableTexture(bool &isFirst);
            void disableShader(bool &isFirst);
        public:
            BufferPreparer(
                    uint_fast32_t memSize, const DevDrawState &state, DPIHelper *helper,
                    ShaderManager *shaderMan,
                    uint_fast32_t width, uint_fast32_t height,
                    uint_fast32_t pxTextureWidth, uint_fast32_t pxTextureHeight,
                    const GenDataVars &vars
            );
            ~BufferPreparer();
            void prepareBuffer(IQueueReader *reader, uint_fast32_t offset, uint_fast32_t maxSize);
            void *getBuffer();
            void clear();
            DrawOpVector::iterator drawOpsBegin();
            DrawOpVector::iterator drawOpsEnd();
            bool isEmpty();
            uint_fast32_t getUsedSize();
            bool isFull();
            uint_fast32_t getLastOffset();
            void resetOffset();
        };

        class DrawOpCreator{
        public:
            template <BufferPreparer::DrawOpType T>
            static BufferPreparer::DrawOp create();
            template <BufferPreparer::DrawOpType T>
            static BufferPreparer::DrawOp create(uint_fast32_t);
            template <BufferPreparer::DrawOpType T>
            static BufferPreparer::DrawOp create(char *userFillPattern);
            template <BufferPreparer::DrawOpType T>
            static BufferPreparer::DrawOp create(
                    uint32_t offset, uint32_t numItems, BufferPreparer::DrawDataType type
            );
            template <BufferPreparer::DrawOpType T>
            static BufferPreparer::DrawOp create(
                    IPixelContainer *container
            );
        };
    }
}


