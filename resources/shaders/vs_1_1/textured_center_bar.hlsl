void main(
        in float4 positionIn : POSITION,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        in float4 texIn0: TEXCOORD0,
        out float4 positionOut : POSITION,
        out float4 colorOut0 : COLOR0,
        out float4 colorOut1 : COLOR1,
        out float4 texOut0: TEXCOORD0,
        out float4 pxShaderPos : TEXCOORD1
){
    positionOut = float4(positionIn.x + texIn0[2] * 2, positionIn.y - texIn0[3] * 2, positionIn.z, 1.0f);
    colorOut0 = colorIn0;
    colorOut1 = colorIn1;
    pxShaderPos = float4(sign(texIn0[2]), sign(texIn0[3]), 0.0f, 1.0f);
    texOut0 = texIn0;
}