#include <main/WException.h>
#include <main/DPIHelper.h>
#include <main/QueueItem.h>
#include "Exception.h"
#include "BufferPreparer.h"
#include "PrimitiveCreator.h"

namespace directgraph{
    namespace dx9{
        BufferPreparer::BufferPreparer(
                uint_fast32_t memSize, DPIHelper *helper,
                const CommonProps &props,
                BufferPreparerParams *bufPrepParams
        ): _bufPrepParams(bufPrepParams), _memSize(memSize), _stateHelper(&_propMan, props), _helper(helper),
           _drawItemProc(&_stateHelper, _bufPrepParams, _helper), _drawStateProc(&_stateHelper, _bufPrepParams, &_propMan),
           _isFirst(true), _curUsedSize(0), _lastOffset(0), _canReadMore(true)
        {
        }

        void BufferPreparer::prepareBuffer(IQueueReader *reader, uint_fast32_t offset, uint_fast32_t maxSize) {
            uint_fast32_t size = reader->getSize();
            uint_fast32_t readIndex = 0;
            if(_patterns.empty() && _stateHelper.getLastState().userFillPattern != NULL){
                _patterns.push_back(_stateHelper.getLastState().userFillPattern);
            }
            for (readIndex = offset; readIndex < size; readIndex++) {
                QueueItem &item = reader->getAt(readIndex);
                if (
                        item.type == QueueItem::SINGLE_PIXEL ||
                        item.type == QueueItem::BAR ||
                        item.type == QueueItem::CLEAR ||
                        item.type == QueueItem::PIXEL_CONTAINER ||
                        item.type == QueueItem::FILLELLIPSE
                ) {
                    ItemState lastState = _drawStateProc.createItemState(item);
                    bool isTransparent = _drawStateProc.isStateTransparent(item, lastState);
                    if(isTransparent){
                        _isFirst = _drawStateProc.processStateDiff(_stateHelper.getCurItemState(), lastState, _transpBuffer.drawOps, true)
                                   || _transpBuffer.vect.empty();
                        _stateHelper.getCurItemState() = lastState;
                    } else {
                        _isFirst = (_drawBuffers.find(lastState) == _drawBuffers.end());
                    }
                    DrawItemProcessor::TypeSize ts = _drawItemProc.getTypeSize(item, lastState);
                    uint_fast32_t curNumVertices = _drawItemProc.getNumVertices(item, _isFirst);
                    uint_fast32_t newUsedSize = _curUsedSize + curNumVertices * ts.sizeMult;
                    if (newUsedSize > maxSize) {
                        _canReadMore = false;
                        break;
                    }
                    _curUsedSize = newUsedSize;
                    if (isTransparent) {
                        void *curMem;
                        if(_transpBuffer.vect.empty()){
                            _transpBuffer.numItems = curNumVertices;
                            _transpBuffer.vect.resize(curNumVertices * ts.sizeMult);
                            curMem = reinterpret_cast<void *>(&_transpBuffer.vect[0]);
                        } else {
                            uint_fast32_t prevSize = static_cast<uint_fast32_t>(_transpBuffer.vect.size());
                            _transpBuffer.numItems += curNumVertices;
                            _transpBuffer.vect.resize(_transpBuffer.vect.size() + curNumVertices * ts.sizeMult);
                            curMem = reinterpret_cast<void *>(&_transpBuffer.vect[0] + prevSize);
                        }
                        if(_isFirst){
                            _transpBuffer.drawOps.push_back(DrawOpCreator::create<DrawOpType::ITEMS>(
                                    curNumVertices * ts.sizeMult,
                                    curNumVertices - VERTICES_TRIANGLES_DIFF,
                                    ts.drawDataType
                            ));
                        } else {
                            _transpBuffer.drawOps.back().data.items.numItems += curNumVertices;
                            _transpBuffer.drawOps.back().data.items.size += curNumVertices * ts.sizeMult;
                        }
                        DrawItemProcessor::StartEndCoords crds = _drawItemProc.getStartEndCoords(item, lastState);
                        if (!_isFirst) {
                            _drawItemProc.genDegenerates(item, curMem, _transpBuffer.prevCrds, crds.start, lastState);
                        }
                        _drawItemProc.processDrawItem(item, curMem, lastState);
                        _transpBuffer.prevCrds = crds.start;
                    } else {
                        ItemsBuffer &buffer = _drawBuffers[lastState];
                        void *curMem;
                        if (_isFirst) {
                            buffer.numItems = curNumVertices;
                            buffer.vect.resize(buffer.numItems * ts.sizeMult);
                            buffer.type = ts.drawDataType;
                            curMem = reinterpret_cast<void *>(&buffer.vect[0]);
                            buffer.offsets.push_back(0);
                        } else {
                            uint_fast32_t prevSize = static_cast<uint_fast32_t>(buffer.vect.size());
                            buffer.numItems += curNumVertices;
                            buffer.vect.resize(buffer.vect.size() + curNumVertices * ts.sizeMult);
                            curMem = reinterpret_cast<void *>(&buffer.vect[0] + prevSize);
                            buffer.offsets.push_back(prevSize);
                        }
                        DrawItemProcessor::StartEndCoords crds = _drawItemProc.getStartEndCoords(item, lastState);
                        _drawItemProc.processDrawItem(item, curMem, lastState);
                        if (!_isFirst) {
                            _drawItemProc.genDegenerates(item, curMem, crds.end, buffer.prevCrds, lastState);
                        }
                        buffer.prevCrds = crds.start;
                    }
                    if(!_drawItemProc.canCreateMoreItems()){
                        _canReadMore = false;
                        break;
                    }
                    _drawItemProc.nextItem();
                } else {
                    switch (item.type){
                        case QueueItem::BGCOLOR:
                            _stateHelper.getLastState().bgColor = item.data.bgColor;
                            break;
                        case QueueItem::SETFILLSTYLE:
                            _stateHelper.getLastState().fillColor = item.data.setfillstyle.color;
                            _stateHelper.getLastState().fillPattern = item.data.setfillstyle.fillStyle;
                            break;
                        case QueueItem::SETFILLPATTERN:
                            _stateHelper.getLastState().fillPattern = USER_FILL;
                            _stateHelper.getLastState().userFillPattern = item.data.setfillpattern.fillPattern;
                            _stateHelper.getLastState().fillColor = item.data.setfillpattern.color;
                            _patterns.push_back(_stateHelper.getLastState().userFillPattern);
                            break;
                        case QueueItem::SETLINESTYLE:
                            _stateHelper.getLastState().lineStyle = item.data.setlinestyle.linestyle;
                            break;
                        default:
                            break;
                    }
                }
            }
            _lastOffset = readIndex;
        }

        void BufferPreparer::genOpsAndMemBlocks() {
            _drawOps.clear();
            _memBlocks.clear();
            _stateHelper.resetState();
            for(BufferMap::iterator it = _drawBuffers.begin(); it != _drawBuffers.end(); ++it){
                ItemState lastState = it->first;
                it->second.offsets.push_back(static_cast<uint_fast32_t>(it->second.vect.size()));
                for(uint_fast32_t i = static_cast<uint_fast32_t>(it->second.offsets.size()) - 2; ; i--){
                    MemBlock block = {
                            &it->second.vect[it->second.offsets[i]],
                            static_cast<uint_fast32_t>(it->second.offsets[i + 1] - it->second.offsets[i])
                    };
                    _memBlocks.push_back(block);
                    if(i == 0){
                        break;
                    }
                }
                _drawStateProc.processStateDiff(_stateHelper.getCurItemState(), lastState, _drawOps, false);
                _stateHelper.getCurItemState() = lastState;
                _drawOps.push_back(DrawOpCreator::create<DrawOpType::ITEMS>(
                        static_cast<uint_fast32_t>(it->second.vect.size()),
                        it->second.numItems - VERTICES_TRIANGLES_DIFF, it->second.type
                ));
            }
            if(!_transpBuffer.vect.empty()) {
                MemBlock transpBlock = {&_transpBuffer.vect[0], static_cast<uint_fast32_t>(_transpBuffer.vect.size())};
                _memBlocks.push_back(transpBlock);
                _drawOps.push_back(DrawOpCreator::create<DrawOpType::START_TRANSPARENT_DATA>());
                std::copy(_transpBuffer.drawOps.begin(), _transpBuffer.drawOps.end(), std::back_inserter(_drawOps));
            }

        }

        void BufferPreparer::clear() {
            _stateHelper.resetState();
            _drawBuffers.clear();
            _transpBuffer.vect.clear();
            _transpBuffer.drawOps.clear();
            if(!_patterns.empty()){
                _patterns.pop_back();
                for(CharPVector::iterator it = _patterns.begin(); it != _patterns.end(); ++it){
                    delete [] *it;
                }
            }
            _patterns.clear();
            _curUsedSize = 0;
            _canReadMore = true;
            _isFirst = true;
            _drawItemProc.resetItemCount();
        }

        DrawOpVector::iterator
        BufferPreparer::drawOpsBegin() {
            return _drawOps.begin();
        }

        DrawOpVector::iterator
        BufferPreparer::drawOpsEnd() {
            return _drawOps.end();
        }

        bool BufferPreparer::isEmpty() {
            return _curUsedSize == 0;
        }

        BufferPreparer::~BufferPreparer() {
            delete [] _stateHelper.getLastState().userFillPattern;
        }

        bool BufferPreparer::isFull() {
            return !_canReadMore;
        }

        uint_fast32_t BufferPreparer::getLastOffset() {
            return _lastOffset;
        }

        void BufferPreparer::resetLastOffset() {
            _lastOffset = 0;
        }

        BufferPreparer::MemBlockVector::iterator BufferPreparer::memBlocksBegin() {
            return _memBlocks.begin();
        }

        BufferPreparer::MemBlockVector::iterator BufferPreparer::memBlocksEnd() {
            return _memBlocks.end();
        }

        uint_fast32_t BufferPreparer::getUsedSize() {
            return _curUsedSize;
        }
    }
}