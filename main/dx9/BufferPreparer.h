#pragma once

#include <stdint.h>
#include <main/IQueueReader.h>
#include <vector>
#include <main/CommonProps.h>
#include "PrimitiveCreator.h"
#include "ShaderManager.h"
#include "BufferPreparerParams.h"
#include "StateHelper.h"
#include "PropertyManager.h"

namespace directgraph {
    namespace dx9 {
        class BufferPreparer {
        public:
            enum DrawOpType{
                REMOVE_PIXEL_TEXTURE,
                REMOVE_PATTERN_TEXTURE,
                SET_FILL_PATTERN,
                SET_FILL_PATTERN_COLOR,
                SET_USER_FILL_PATTERN,
                SET_TEX_BG_COLOR,
                ITEMS,
                SET_PIXEL_TEXTURE,
                SET_LINE_STYLE
            };
            enum DrawDataType{
                COLOR_VERTEX,
                TEXTURED_COLOR_VERTEX,
                TEXTURED_VERTEX,
                COLOR2_VERTEX,
                ELLIPSE_VERTEX,
                TEXTURED_ELLIPSE_VERTEX
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
            DrawOpVector _drawOps;
            CharPVector _patterns;
            StateHelper _stateHelper;
            PropertyManager _propMan;
            DPIHelper *_helper;
            bool _isFirst;
            int_fast32_t _prevX, _prevY;
            uint_fast32_t _curUsedSize;
            uint_fast32_t _lastOffset;
            bool _canReadMore;
            ItemState createItemState(const QueueItem &item);
            bool processStateDiff(const ItemState &statePrev, const ItemState &stateCur);
            struct TypeSize{
                uint_fast32_t sizeMult;
                DrawDataType drawDataType;
            };
            TypeSize getTypeSize(const QueueItem &item);
            void processDrawItem(
                    const QueueItem &item, void *&curVertMem
            );
            void useFillTexture(ItemState &state, bool useBgColor);
            void disablePixelTexture(ItemState &state);
            void disableTexture(ItemState &state);
            void useLineStyle(ItemState &state);
            void disableShader(ItemState &state);
        public:
            BufferPreparer(
                    uint_fast32_t memSize, DPIHelper *helper,
                    const CommonProps &props,
                    BufferPreparerParams *bufPrepParams
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
            void resetLastOffset();
            void endDraw();

            void genDegenerates(const QueueItem &item, void *&curVertMem);
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


