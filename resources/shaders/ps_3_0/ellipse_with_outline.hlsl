#include <common_ps.hlsl>

void main(
        in float thickness: TEXCOORD0,
        in float4 positionIn: TEXCOORD1,
        in float4 colorOutline : COLOR0,
        in float4 colorEllipse : COLOR1,
        out float4 colorOut : COLOR
){
    float dist = ellipseDistance(abs(positionIn.xy), positionIn.zw);
    if(dist > (thickness / 2)){
        discard;
    }
    if(abs(dist) > thickness / 2){
        colorOut = colorEllipse;
    } else {
        colorOut = colorOutline;
    }
}