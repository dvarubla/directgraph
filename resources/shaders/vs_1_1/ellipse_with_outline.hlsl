#include <common_vs.hlsl>

void main(
        in float4 positionIn : POSITION,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        in float2 offsets : TEXCOORD0,
        uniform float2 maxDimens: register(c0),
        out float4 positionOut : POSITION,
        out float4 colorOut0 : COLOR0,
        out float4 colorOut1 : COLOR1,
        out float thickness : TEXCOORD0,
        out float4 pxShaderPos : TEXCOORD1
){
    thickness = positionIn[3];
    float2 offset = offsets + sign(offsets) * thickness / 2;
    positionOut = transformPositionCenter(positionIn.xyz, offset, maxDimens);
    colorOut0 = colorIn0;
    colorOut1 = colorIn1;
    pxShaderPos = float4(offset.x, offset.y, abs(offsets.x), abs(offsets.y));
}