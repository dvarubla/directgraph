void clipEllipse(float2 position){
    clip(1.0f - position.x * position.x - position.y * position.y);
}

float4 sampleFillPattern(float4 colorIn0, float4 colorIn1, sampler texSampler, float2 tex){
    return lerp(colorIn0, colorIn1, tex2D(texSampler, tex).a);
}