#include <common_ps.hlsl>

void main(
        in float thickness: TEXCOORD0,
        in float4 positionIn: TEXCOORD1,
        in float4 colorIn : COLOR,
        out float4 colorOut : COLOR
){
    float dist = ellipseDistance(abs(positionIn.xy), positionIn.zw);
    clip((thickness / 2) - abs(dist));
    colorOut = colorIn;
}