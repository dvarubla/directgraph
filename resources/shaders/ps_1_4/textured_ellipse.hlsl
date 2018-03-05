sampler texSampler;

void main(
        in float2 texIn0: TEXCOORD0,
        in float4 positionIn: TEXCOORD1,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        out float4 colorOut : COLOR
){
    clip(1.0f - positionIn.x * positionIn.x - positionIn.y * positionIn.y);
    colorOut = lerp(colorIn0, colorIn1, tex2D(texSampler, texIn0).a);
}