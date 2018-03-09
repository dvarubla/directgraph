void main(
        in float3 positionIn : POSITION,
        in float2 texIn0 : TEXCOORD0,
        in float4 colorIn : COLOR,
        out float4 positionOut : POSITION,
        out float4 colorOut : COLOR,
        out float4 pxShaderPos : TEXCOORD1
){
    positionOut = float4(positionIn.x + texIn0.x * 2, positionIn.y - texIn0.y * 2, positionIn.z, 1.0f);
    colorOut = colorIn;
    pxShaderPos = float4(sign(texIn0.x), sign(texIn0.y), 0.0f, 1.0f);
}