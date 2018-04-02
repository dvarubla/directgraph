#include <directgraph/directgraph_constants.h>
#include <stdlib.h>
#include "CommonProps.h"

namespace directgraph{
    CommonProps get_default_common_props() {
        CommonProps props;
        props.fillStyle = SOLID_FILL;
        props.fillColor = 0xFFFFFF;
        props.bgColor = 0xFFFFFF;
        props.userFillPattern = NULL;
        props.lineStyle = SOLID_LINE;
        props.drawColor = 0x000000;
        props.lineThickness = 1;
        props.userLinePattern = 0;
        props.curPos.x = 0;
        props.curPos.y = 0;
        return props;
    }
}
