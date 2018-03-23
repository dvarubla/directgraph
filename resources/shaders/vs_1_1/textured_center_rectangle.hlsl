#include <common_vs.hlsl>

void main(
        in float4 positionIn : POSITION,
        in float4 colorIn0 : COLOR0,
        in float2 rectDimens: TEXCOORD0,
        uniform float2 maxDimens: register(c0),
        out float4 positionOut : POSITION,
        out float4 colorOut : COLOR0,
        out float4 texOut0 : TEXCOORD0,
        out float4 texOut1 : TEXCOORD1,
        out float4 texOut2 : TEXCOORD2,
        out float4 texOut3 : TEXCOORD3
){
    float thickness = positionIn.w;
    float halfThickness = floor(thickness / 2);

    positionOut = transformPosition(
        positionIn.xyz +
            sign(float3(rectDimens.x, rectDimens.y, 0)) * float3(halfThickness, halfThickness, 0),
        maxDimens
    );
    colorOut = colorIn0;

    float2 rectDimensAbs = abs(rectDimens);
    float2 rectDimensThicknessSigned = rectDimens + sign(rectDimens) * 2 * halfThickness;
    float2 rectDimensThickness = abs(rectDimensThicknessSigned);

    float2 halfAndThickness = float2(
        floor(rectDimensAbs.x / 2) + halfThickness, floor(rectDimensAbs.y / 2) + halfThickness
    );

    float4 limits;
    limits.xy = max(
        halfAndThickness + float2(rectDimensAbs.x % 2, rectDimensAbs.y % 2),
        float2(rectDimensThickness.x, rectDimensThickness.y) - thickness
    );
    limits.zw = min(
        halfAndThickness, thickness
    );

    float2 texCrd = (rectDimensThickness + rectDimensThicknessSigned) / 2;

    texOut0.xy = -(texCrd - float2(limits[0], limits[3])) / 16.0f;
    texOut0.zw = float2(limits[0] / 16.0f + 1, 1);

    float len = frac(limits[0] / 16.0f);
    texOut1.yx = (texCrd - float2(limits[0], limits[1])) / 16.0f;
    texOut1.x *= -1;
    texOut1.zw = float2(limits[1] / 16.0f + len + 1, 1);

    len = frac(len + limits[1] / 16.0f);
    texOut2.xy = (texCrd - float2(limits[2], limits[1])) / 16.0f;
    texOut2.zw = float2((rectDimensThickness.x - limits[2]) / 16.0f + len + 1, 1);

    len = frac(len + (rectDimensThickness.x - limits[2]) / 16.0f);
    texOut3.yx = (texCrd - float2(limits[2], limits[3])) / 16.0f;
    texOut3.y *= -1;
    texOut3.zw = float2((rectDimensThickness.y - limits[3]) / 16.0f + len + 1, 1);
}