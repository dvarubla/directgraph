#pragma once

#include <directgraph/directgraph_constants.h>

namespace directgraph{
    namespace dx9{
        class IPatternTexturesHelper{
        public:
            virtual void setBgColor(uint_fast32_t bgColor) = 0;
            virtual void setFillPattern(uint_fast8_t pattern) = 0;
            virtual void setFillPatternBgColor(uint_fast8_t pattern, uint_fast32_t bgColor) = 0;
            virtual void setUserFillPattern(const char *pattern) = 0;
            virtual void unsetPattern() = 0;
            virtual ~IPatternTexturesHelper(){};
        };
    }
}

