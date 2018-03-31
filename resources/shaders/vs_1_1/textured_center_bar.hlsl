#include <common_vs.hlsl>

void main(
        in float3 positionIn : POSITION,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        in float2 texIn0: TEXCOORD0,
        uniform float2 maxDimens: register(c0),
        out float4 positionOut : POSITION,
        out float4 colorOut0 : COLOR0,
        out float4 colorOut1 : COLOR1,
        out float2 texOut0: TEXCOORD0,
        out float4 pxShaderPos : TEXCOORD1
){
    positionOut = transformPositionCenter(positionIn, texIn0, maxDimens);
    colorOut0 = colorIn0;
    colorOut1 = colorIn1;
    pxShaderPos = float4(sign(texIn0.x), sign(texIn0.y), 0.0f, 1.0f);
    texOut0 = (abs(texIn0.xy) + texIn0.xy + 0.5f) / 8.0f ;
}