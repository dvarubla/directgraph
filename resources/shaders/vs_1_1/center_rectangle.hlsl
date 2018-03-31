#include <common_vs.hlsl>

void main(
        in float4 positionIn : POSITION,
        in float4 colorIn0 : COLOR0,
        in float2 rectDimens: TEXCOORD0,
        uniform float2 maxDimens: register(c0),
        out float4 positionOut : POSITION,
        out float4 colorOut : COLOR0,
        out float2 texOut0 : TEXCOORD0,
        out float2 texOut1 : TEXCOORD1
){
    float thickness = positionIn.w;
    float halfThickness = floor(thickness / 2);

    positionOut = transformPosition(
        subtOffset(positionIn.xyz) +
            sign(float3(rectDimens.x, rectDimens.y, 0)) * float3(halfThickness, halfThickness, 0),
        maxDimens
    );
    colorOut = colorIn0;

    float2 rectDimensOutside = rectDimens / 2 + sign(rectDimens) * halfThickness;
    float2 rectDimensInside = abs(rectDimensOutside) - thickness;

    texOut0 = rectDimensOutside;
    texOut1 = rectDimensInside;
}