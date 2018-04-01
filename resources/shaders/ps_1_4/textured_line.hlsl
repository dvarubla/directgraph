sampler texSampler;

void main(
        in float2 tex0: TEXCOORD0,
        in float4 colorIn : COLOR0,
        out float4 colorOut : COLOR
){
    if(tex1D(texSampler, tex0.x).a == 1.0f){
        discard;
    } else {
        colorOut = colorIn;
    }
}