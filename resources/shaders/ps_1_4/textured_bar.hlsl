#include <common_ps.hlsl>

sampler texSampler;

void main(
        in float2 tex0: TEXCOORD0,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        out float4 colorOut : COLOR
){
    colorOut = sampleFillPattern(colorIn0, colorIn1, texSampler, tex0);
}