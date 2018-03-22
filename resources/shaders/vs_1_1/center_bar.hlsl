#include <common_vs.hlsl>

void main(
        in float3 positionIn : POSITION,
        in float2 texIn0 : TEXCOORD0,
        in float4 colorIn : COLOR,
        uniform float2 maxDimens: register(c0),
        out float4 positionOut : POSITION,
        out float4 colorOut : COLOR,
        out float2 pxShaderPos : TEXCOORD1
){
    positionOut = transformPositionCenter(positionIn, texIn0, maxDimens);
    colorOut = colorIn;
    pxShaderPos = sign(texIn0);
}