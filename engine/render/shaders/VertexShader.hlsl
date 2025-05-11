cbuffer MVP : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

struct VSInput {
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float3 color    : COLOR;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float3 color    : COLOR;
};

VSOutput main(VSInput input) {
    VSOutput output;

    float4 worldPos = mul(model, float4(input.position, 1.0f));
    float4 viewPos = mul(view, worldPos);

    output.position = mul(projection, viewPos);
    output.color = input.color;

    float3 worldNormal = mul((float3x3)model, input.normal); // correct for rotation/scale only
    output.normal = normalize(worldNormal);

    return output;
}
