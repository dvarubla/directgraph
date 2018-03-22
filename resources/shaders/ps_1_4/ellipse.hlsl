#include <common_ps.hlsl>

void main(
        in float4 positionIn: TEXCOORD1,
        in float4 colorIn : COLOR,
        out float4 colorOut : COLOR
){
    clipEllipse(positionIn.xy);
    colorOut = colorIn;
}