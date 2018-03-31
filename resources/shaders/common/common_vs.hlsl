float4 transformPosition(float3 positionIn, float2 maxDimens){
    return float4(
                2.f * ((positionIn.x) / maxDimens.x) - 1.f,
                1.f - 2.f * ((positionIn.y) / maxDimens.y),
                positionIn.z,
                1.0f
    );
}

float4 transformPositionCenter(float3 positionIn, float2 offsets, float2 maxDimens){
    return float4(
                2.f * ((positionIn.x + offsets.x) / maxDimens.x) - 1.f,
                1.f - 2.f * ((positionIn.y + offsets.y) / maxDimens.y),
                positionIn.z,
                1.0f
    );
}

float3 subtOffset(float3 pos){
    return float3(pos.x - 0.5f, pos.y - 0.5f, pos.z);
}