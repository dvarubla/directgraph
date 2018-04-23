#include <common_ps.hlsl>

sampler texSampler;

void main(
        in float3 thicknessAndTexCoords: TEXCOORD0,
        in float4 positionIn: TEXCOORD1,
        in float4 colorOutline : COLOR0,
        in float4 colorEllipse : COLOR1,
        in float4 colorEllipse2 : TEXCOORD2,
        out float4 colorOut : COLOR
){
    float dist = ellipseDistance(abs(positionIn.xy), positionIn.zw);
    float thickness = thicknessAndTexCoords[2];
    float2 texCoords = thicknessAndTexCoords.xy;
    if(dist > (thickness / 2)){
        discard;
    }
    if(abs(dist) > thickness / 2){
        colorOut = sampleFillPattern(colorEllipse, colorEllipse2, texSampler, texCoords);
    } else {
        colorOut = colorOutline;
    }
}