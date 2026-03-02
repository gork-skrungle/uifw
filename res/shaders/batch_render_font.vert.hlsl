struct FontGlyphInstance
{
  float3 position;
  float2 size;
  float2 _padding; // Padding variable to satisfy GLSL std140 layout requirement (extra 16 bytes)
  float4 texCoords;
  float4 color;
};

struct Output
{
  float2 texCoord : TEXCOORD0;
  float4 color : TEXCOORD1;
  float4 position : SV_Position;
};

StructuredBuffer<FontGlyphInstance> DataBuffer : register(t0, space0);

cbuffer UniformBlock : register(b0, space1)
{
  float4x4 viewProjectionMatrix : packoffset(c0);
}

static const uint triangleIndices[6] = {0, 1, 2, 3, 2, 1};
static const float2 vertexPos[4] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f}
};

Output main(uint id : SV_VertexID)
{
    uint spriteIndex = id / 6;
    uint vert = triangleIndices[id % 6];

    FontGlyphInstance sprite = DataBuffer[spriteIndex];
    
    float2 texCoord[4] = {
      {sprite.texCoords.x, sprite.texCoords.y},
      {sprite.texCoords.x + sprite.texCoords.z, sprite.texCoords.y},
      {sprite.texCoords.x, sprite.texCoords.z + sprite.texCoords.w},
      {sprite.texCoords.x + sprite.texCoords.z, sprite.texCoords.z + sprite.texCoords.w}
    };

    float2 coord = vertexPos[vert];
    coord *= sprite.size;

    float3 coordWithDepth = float3(coord + sprite.position.xy, sprite.position.z);

    Output output;

    output.position = mul(viewProjectionMatrix, float4(coordWithDepth, 1.0f));
    output.texCoord = texCoord[vert];
    output.color = sprite.color;

    return output;
}