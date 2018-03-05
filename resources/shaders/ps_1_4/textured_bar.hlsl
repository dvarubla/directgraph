sampler texSampler;

void main(
        in float2 tex0: TEXCOORD0,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        out float4 colorOut : COLOR
){
    colorOut = lerp(colorIn0, colorIn1, tex2D(texSampler, tex0).a);
}