#pragma once

#include <directgraph/directgraph_constants.h>

namespace directgraph{
    namespace dx9{
        class IPatternTexturesHelper{
        public:
            virtual void setBgColor(uint_fast32_t bgColor) = 0;
            virtual void setFillPattern(uint_fast8_t pattern, bool useTransparency) = 0;
            virtual void setLinePattern(uint_fast8_t pattern, bool useTransparency) = 0;
            virtual void setLinePatternColor(uint_fast8_t pattern, uint_fast32_t color, bool useTransparency) = 0;
            virtual void setUserLinePattern(uint_fast16_t pattern) = 0;
            virtual void setFillPatternBgColor(uint_fast8_t pattern, uint_fast32_t bgColor, bool useTransparency) = 0;
            virtual void setFillPatternBgFillColor(uint_fast8_t pattern, uint_fast32_t bgColor, uint_fast32_t fillColor) = 0;
            virtual void setUserFillPattern(const char *pattern) = 0;
            virtual void unsetPattern(bool useTransparency) = 0;
            virtual ~IPatternTexturesHelper(){};
        };
    }
}

