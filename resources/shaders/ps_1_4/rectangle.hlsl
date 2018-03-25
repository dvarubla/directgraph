sampler texSampler;

void main(
        in float2 tex0: TEXCOORD0,
        in float2 tex1: TEXCOORD1,
        in float4 colorIn: COLOR0,
        out float4 colorOut: COLOR
){
    float2 vect = sign(abs(tex0) - tex1);
    clip(vect.x + vect.y);
    colorOut = colorIn;
}