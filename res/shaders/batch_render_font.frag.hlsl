Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

struct Input
{
  float2 texCoord : TEXCOORD0;
  float4 color : TEXCOORD1;
};

float median(float a, float b, float c)
{
  return max(min(a, b), min(max(a, b), c));
}

float screenPxRange(float2 texCoord) 
{
  const float pxRange = 2; // 2px defined in msdf-atlas-gen settings
  const float texSize = 1004; // Hard-coded from msdf-atlas-gen settings

  float2 unitRange = float2(pxRange, pxRange) / float2(texSize, texSize);
  float2 screenTexSize = float2(1.0, 1.0) / fwidth(texCoord);

  return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float4 main(Input input) : SV_Target0
{
  const float4 bgColor = float4(0.0, 0.0, 0.0, 0.0);

  float3 msd = Texture.Sample(Sampler, input.texCoord).rgb;
  float sd = median(msd.r, msd.g, msd.b);
  float screenPxDistance = screenPxRange(input.texCoord) * (sd - 0.5);
  float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

  return float4(input.color.rgb, opacity);
}