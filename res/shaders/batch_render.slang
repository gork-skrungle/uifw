struct SpriteData {
    float3 position;
    float rotation;
    float2 scale;
    float2 _padding;
    float4 texCoords;
    float4 color;
};

struct VertexOutput {
    float2 texCoord : TEXCOORD0;
    float4 color : TEXCOORD1;
    float4 position: SV_Position;
};

struct FragmentOutput {
    float4 color;
}

[[vk::binding(0, 0)]]
StructuredBuffer<SpriteData> DataBuffer : register(t0, space0);

[[vk::binding(0, 1)]]
cbuffer UniformBlock : register(b0, space1) {
    float4x4 viewProjectionMatrix : packoffset(c0);
}

static const uint triangleIndices[6] = {0, 1, 2, 3, 2, 1};
static const float2 vertexPos[4] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f}
};

[shader("vertex")]
VertexOutput vertMain(uint id : SV_VertexID) {
    uint spriteIndex = id / 6;
    uint vert = triangleIndices[id % 6];
    SpriteData sprite = DataBuffer[spriteIndex];

    float2 texCoord[4] = {
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f}
    };

    float c = cos(sprite.rotation);
    float s = sin(sprite.rotation);

    float2 coord = vertexPos[vert];
    coord *= sprite.scale;
    float2x2 rotation = {c, s, -s, c};
    coord = mul(coord, rotation);

    float3 coordWithDepth = float3(
        coord + sprite.position.xy,
        sprite.position.z
    );

    VertexOutput output;

    output.position = mul(viewProjectionMatrix, float4(coordWithDepth, 1.0));
    output.texCoord = texCoord[vert];
    output.color = sprite.color;

    return output;
}

[shader("fragment")]
FragmentOutput fragmentMain(VertexOutput vertex) : SV_Target {
    FragmentOutput output;
    output.color = vertex.color;
    return output;
}