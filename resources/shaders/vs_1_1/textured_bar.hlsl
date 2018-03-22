#include <common_vs.hlsl>

void main(
        in float3 positionIn : POSITION,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        in float2 texIn0 : TEXCOORD0,
        uniform float2 maxDimens: register(c0),
        out float4 positionOut : POSITION,
        out float4 colorOut0 : COLOR0,
        out float4 colorOut1 : COLOR1,
        out float2 texOut0 : TEXCOORD0
){
    positionOut = transformPosition(positionIn, maxDimens);
    colorOut0 = colorIn0;
    colorOut1 = colorIn1;
    texOut0 = texIn0;
}