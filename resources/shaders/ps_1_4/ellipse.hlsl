void main(
        in float4 positionIn: TEXCOORD1,
        in float4 colorIn : COLOR,
        out float4 colorOut : COLOR
){
    clip(1.0f - positionIn.x * positionIn.x - positionIn.y * positionIn.y);
    colorOut = colorIn;
}