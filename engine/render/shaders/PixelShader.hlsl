struct PSInput {
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float3 color    : COLOR;
};

float4 main(PSInput input) : SV_TARGET {
    float3 lightDir = normalize(float3(0.5f, 1.0f, 0.3f)); // example directional light
    float lightIntensity = max(dot(normalize(input.normal), lightDir), 0.0f);

    float3 litColor = input.color * lightIntensity;
    return float4(litColor, 1.0f);
}
