#pragma once

#include <stdint.h>
#include <main/IQueueReader.h>
#include <vector>
#include <unordered_map>
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
                START_TRANSPARENT_DATA,
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
                        uint32_t size;
                        uint32_t numItems;
                        DrawDataType type;
                    } items;
                    uint8_t lineStyle;
                    IPixelContainer *pixelContainer;
                } data;
            };
            typedef std::vector<DrawOp> DrawOpVector;
            typedef std::vector<char*> CharPVector;
            typedef std::vector<uint8_t> ByteVector;
            typedef std::vector<uint32_t> OffsetVector;
            struct MemBlock{
                uint8_t *mem;
                uint_fast32_t size;
            };
            typedef std::vector<MemBlock> MemBlockVector;
            struct ItemsBuffer{
                ByteVector vect;
                OffsetVector offsets;
                uint32_t numItems;
                uint32_t sizeMult;
                DrawDataType type;
                Coords prevCrds;
            };

            const static int VERTICES_TRIANGLES_DIFF = 2;
        private:
            typedef std::unordered_map<ItemState, ItemsBuffer, ItemStateHash> BufferMap;
            struct {
                ByteVector vect;

                uint32_t numItems;
                DrawOpVector drawOps;
                Coords prevCrds;
            } _transpBuffer;
            const static int TRIANGLES_IN_QUAD = 2;
            const static int VERTICES_IN_QUAD = 4;

            BufferPreparerParams *_bufPrepParams;
            PrimitiveCreator _primCreator;
            uint_fast32_t _memSize;
            BufferMap _drawBuffers;
            DrawOpVector _drawOps;
            MemBlockVector _memBlocks;
            CharPVector _patterns;
            StateHelper _stateHelper;
            PropertyManager _propMan;
            DPIHelper *_helper;
            bool _isFirst;
            uint_fast32_t _itemNum;
            float _curZ;
            uint_fast32_t _curUsedSize;
            uint_fast32_t _lastOffset;
            bool _canReadMore;
            ItemState createItemState(const QueueItem &item);
            bool isStateTransparent(const QueueItem &item, const ItemState &state);
            bool processStateDiff(
                    const ItemState &statePrev, const ItemState &stateCur,
                    DrawOpVector &drawOps
            );
            struct TypeSize{
                uint_fast32_t sizeMult;
                DrawDataType drawDataType;
            };
            struct StartEndCoords{
                Coords start;
                Coords end;
            };
            TypeSize getTypeSize(const QueueItem &item, const ItemState &state);
            void processDrawItem(
                    const QueueItem &item, void *&curVertMem,
                    const ItemState &state
            );
            StartEndCoords getStartEndCoords(
                    const QueueItem &item, const ItemState &state
            );
            void useFillTexture(ItemState &state, bool useBgColor);
            void disablePixelTexture(ItemState &state);
            void disableTexture(ItemState &state);
            void useLineStyle(ItemState &state);
            void disableShader(ItemState &state);
            uint_fast32_t getNumVertices(const QueueItem &item);
        public:
            BufferPreparer(
                    uint_fast32_t memSize, DPIHelper *helper,
                    const CommonProps &props,
                    BufferPreparerParams *bufPrepParams
            );
            ~BufferPreparer();
            void prepareBuffer(IQueueReader *reader, uint_fast32_t offset, uint_fast32_t maxSize);
            void clear();
            void genOpsAndMemBlocks();
            DrawOpVector::iterator drawOpsBegin();
            DrawOpVector::iterator drawOpsEnd();

            MemBlockVector::iterator memBlocksBegin();
            MemBlockVector::iterator memBlocksEnd();
            bool isEmpty();
            uint_fast32_t getUsedSize();
            bool isFull();
            uint_fast32_t getLastOffset();
            void resetLastOffset();

            void genDegenerates(
                    const QueueItem &item, void *&curVertMem,
                    const Coords &startCrds, const Coords &endCrds,
                    const ItemState &state
            );
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
                    uint32_t size, uint32_t numItems, BufferPreparer::DrawDataType type
            );
            template <BufferPreparer::DrawOpType T>
            static BufferPreparer::DrawOp create(
                    IPixelContainer *container
            );
        };
    }
}


