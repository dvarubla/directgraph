#include <directgraph/directgraph_constants.h>
#include "CommonProps.h"
#include <cstdlib>

namespace directgraph{
    CommonProps get_default_common_props() {
        CommonProps props;
        props.fillStyle = SOLID_FILL;
        props.fillColor = 0xFFFFFF;
        props.bgColor = 0xFFFFFF;
        props.userFillPattern = NULL;
        props.lineStyle = SOLID_LINE;
        return props;
    }
}
