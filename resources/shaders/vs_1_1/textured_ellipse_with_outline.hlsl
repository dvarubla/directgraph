#include <common_vs.hlsl>

void main(
        in float4 positionIn : POSITION,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        in float4 colorIn2 : COLOR2,
        in float2 offsets : TEXCOORD0,
        uniform float2 maxDimens: register(c0),
        out float4 positionOut : POSITION,
        out float4 colorOut0 : COLOR0,
        out float4 colorOut1 : COLOR1,
        out float4 colorOut2 : TEXCOORD2,
        out float3 thicknessAndTexCoords : TEXCOORD0,
        out float4 pxShaderPos : TEXCOORD1
){
    float thickness = positionIn[3];
    float2 offset = offsets + sign(offsets) * thickness / 2;
    positionOut = transformPositionCenter(positionIn.xyz, offset, maxDimens);
    colorOut0 = colorIn0;
    colorOut1 = colorIn1;
    colorOut2 = colorIn2;
    float2 texCrds = (abs(offset.xy) + offset.xy + thickness + 0.5f) / 8.0f;
    thicknessAndTexCoords = float3(texCrds.x, texCrds.y, thickness);
    pxShaderPos = float4(offset.x, offset.y, abs(offsets.x), abs(offsets.y));
}