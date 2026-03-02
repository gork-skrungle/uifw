Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

struct Input
{
  float2 texCoord : TEXCOORD0;
  float4 color : TEXCOORD1;
};

float4 main(Input input) : SV_Target0
{
  return input.color * Texture.Sample(Sampler, input.texCoord);
}
