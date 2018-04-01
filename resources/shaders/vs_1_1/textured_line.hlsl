#include <common_vs.hlsl>

void main(
        in float4 positionIn : POSITION,
        in float4 colorIn : COLOR0,
        uniform float2 maxDimens: register(c0),
        out float4 positionOut : POSITION,
        out float4 colorOut : COLOR0,
        out float2 texOut : TEXCOORD0
){
    positionOut = transformPosition(positionIn, maxDimens);
    colorOut = colorIn;
    texOut = positionIn.w;
}