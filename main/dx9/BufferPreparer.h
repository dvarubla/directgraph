#pragma once

#include <stdint.h>
#include <main/IQueueReader.h>
#include <vector>
#include "PrimitiveCreator.h"
#include "ShaderManager.h"
#include "BufferPreparerParams.h"

namespace directgraph {
    namespace dx9 {
        class BufferPreparer {
        public:
            enum DrawOpType{
                CLEAR,
                REMOVE_TEXTURE,
                SET_FILL_PATTERN,
                SET_FILL_PATTERN_COLOR,
                SET_USER_FILL_PATTERN,
                SET_TEX_BG_COLOR,
                ITEMS,
                SET_PIXEL_TEXTURE,
                SET_LINE_STYLE
            };
            enum DrawDataType{
                RECT_VERTEX,
                TEXTURED_RECT_VERTEX,
                TEXTURED_VERTEX,
                COLOR2_VERTEX,
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
                    struct FillPatternColor {
                        uint8_t fillPattern;
                        uint32_t bgColor;
                    } fillPatternColor;
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

            BufferPreparerParams *_bufPrepParams;
            PrimitiveCreator _primCreator;
            void *_vertMem;
            uint_fast32_t _memSize;
            DevDrawState _lastState, _curState;
            DrawOpVector _drawOps;
            CharPVector _patterns;
            DPIHelper *_helper;
            GenDataVars _curGenDataVars;
            struct CurSettings{
                bool textureSet;
                bool bgColorSet;
                bool fillPatternRecreated;
                ShaderType shaderType;
            } _curSettings;
            bool _isFirst;
            uint_fast32_t _curUsedSize;
            uint_fast32_t _lastOffset;
            bool _canReadMore;
            void processStateChanges(const QueueItem &item);
            struct TypeSize{
                uint_fast32_t sizeMult;
                DrawDataType drawDataType;
            };
            TypeSize getTypeSize(const QueueItem &item);
            void processDrawItem(
                    const QueueItem &item, void *&curVertMem, int_fast32_t &prevX, int_fast32_t &prevY
            );
            void useFillTexture(bool useBgColor);
            void disableTexture();
            void disableShader();
            void setInitialSettings();
        public:
            BufferPreparer(
                    uint_fast32_t memSize, const DevDrawState &state, DPIHelper *helper,
                    BufferPreparerParams *bufPrepParams,
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
            static BufferPreparer::DrawOp create(uint_fast32_t, uint_fast32_t);
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


