void main(
        in float4 positionIn : POSITION,
        uniform float4 coords: register(c0),
        uniform float4 colorIn: register(c1),
        uniform float4 extraParams: register(c2),
        out float4 positionOut : POSITION,
        out float4 colorOut : COLOR
){
    float2 dimens = extraParams.xy;
    float2x2 coordMatrix = {coords[0], coords[1], coords[2], coords[3]};
    float4x4 resMatrix = 0.0f;

    resMatrix._11 = (coordMatrix._21 - coordMatrix._11) / dimens.x;
    resMatrix._22 = (coordMatrix._12 - coordMatrix._22) / dimens.y;
    resMatrix._14 = 2 * coordMatrix._21 / dimens.x - 1 - resMatrix._11;
    resMatrix._24 = - 2 * coordMatrix._12 / dimens.y + resMatrix._22 + 1;
    resMatrix._44 = 1;
    positionOut = mul(resMatrix, float4(sign(positionIn[0]), sign(abs(positionIn[0]) - 0.5f), 0.0f, 1.0f));
    colorOut = colorIn;
}