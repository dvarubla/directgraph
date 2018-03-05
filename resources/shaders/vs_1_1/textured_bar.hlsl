void main(
        in float4 positionIn : POSITION,
        in float4 colorIn0 : COLOR0,
        in float4 colorIn1 : COLOR1,
        out float4 positionOut : POSITION,
        out float4 colorOut0 : COLOR0,
        out float4 colorOut1 : COLOR1,
        out float4 texOut0: TEXCOORD0
){
    positionOut = float4(positionIn.x, positionIn.y, 0.0f, 1.0f);
    colorOut0 = colorIn0;
    colorOut1 = colorIn1;
    texOut0 = float4(positionIn[2], positionIn[3], 0.0f, 0.0f);
}