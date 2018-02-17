void main(
        in float4 positionIn : POSITION,
        in float4 colorIn : COLOR,
        out float4 positionOut : POSITION,
        out float4 colorOut : COLOR,
        out float4 pxShaderPos : TEXCOORD1
){
    positionOut = float4(positionIn.x + positionIn[2] * 2, positionIn.y - positionIn[3] * 2, 0.0f, 1.0f);
    colorOut = colorIn;
    pxShaderPos = float4(sign(positionIn[2]), sign(positionIn[3]), 0.0f, 1.0f);
}