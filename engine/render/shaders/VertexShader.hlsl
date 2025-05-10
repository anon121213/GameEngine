cbuffer MVP : register(b0)
{
  matrix model;
  matrix view;
  matrix projection;
};

struct VSInput {
  float3 position : POSITION;
  float3 color    : COLOR;
};

struct VSOutput {
  float4 position : SV_POSITION;
  float3 color    : COLOR;
};

VSOutput main(VSInput input) {
  VSOutput output;
  float4 world = mul(model, float4(input.position, 1.0f));
  float4 viewPos = mul(view, world);
  output.position = mul(projection, viewPos);
  output.color = input.color;
  return output;
}
