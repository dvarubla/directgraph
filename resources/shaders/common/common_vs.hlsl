float4 transformPosition(float3 positionIn, float2 maxDimens){
    return float4(
                2.f * ((positionIn.x - 0.5f) / maxDimens.x) - 1.f,
                1.f - 2.f * ((positionIn.y - 0.5f) / maxDimens.y),
                positionIn.z,
                1.0f
    );
}

float4 transformPositionCenter(float3 positionIn, float2 offsets, float2 maxDimens){
    return float4(
                2.f * ((positionIn.x + offsets.x - 0.5f) / maxDimens.x) - 1.f,
                1.f - 2.f * ((positionIn.y + offsets.y - 0.5f) / maxDimens.y),
                positionIn.z,
                1.0f
    );
}