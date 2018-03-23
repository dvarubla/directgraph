sampler texSampler;

void main(
        in float4 tex0: TEXCOORD0,
        in float4 tex1: TEXCOORD1,
        in float4 tex2: TEXCOORD2,
        in float4 tex3: TEXCOORD3,
        in float4 colorIn : COLOR0,
        out float4 colorOut : COLOR
){
    float4x4 matr;
    matr[0] = tex0;
    matr[1] = tex1;
    matr[2] = tex2;
    matr[3] = tex3;
    matr = clamp(matr, 0, 100000000000.);
    matr[0] *= all(matr[0]);
    matr[1] *= all(matr[1]);
    matr[2] *= all(matr[2]);
    matr[3] *= all(matr[3]);
    float4 vect = mul(float4(1, 1, 1, 1), matr);
    if(vect.x == 0 || tex1D(texSampler, vect.z - vect.x).a == 1.0f){
        discard;
    } else {
        colorOut = colorIn;
    }
}