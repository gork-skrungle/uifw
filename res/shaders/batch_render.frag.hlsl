struct Input
{
  float4 color : TEXCOORD0;
  float4 borderRadius : TEXCOORD1;
  float4 spriteBounds : TEXCOORD2;
  float3 fragmentPosition : TEXCOORD3;
};

// ---- Corner states ---- //
static const int CORNER_NONE = -1;
static const int CORNER_TOP_LEFT = 0;
static const int CORNER_TOP_RIGHT = 1;
static const int CORNER_BOTTOM_RIGHT = 2;
static const int CORNER_BOTTOM_LEFT = 3;
// ---------------------- //

// Detect if the current fragment is in a corner. Return integer defining
//  which corner the fragment is currently in.
//
//  -1 = Not in corner
//   0 = Top-left
//   1 = Top-right
//   2 = Bottom-left
//   3 = Bottom-right
float get_rounded_rect_smooth(float3 fragmentPosition, float4 spriteBounds, float4 borderRadius) {
  float2 halfSize = spriteBounds.zw * 0.5f;
  float2 center = spriteBounds.xy + halfSize;
  float2 p = fragmentPosition.xy - center;
  
  float2 s = float2(p.x >= 0.0f ? 1.0f : -1.0f, p.y >= 0.0f ? 1.0f : -1.0f);
  int ix = (s.x >= 0.0f) ? 1 : 0;
  int iy = (s.y >= 0.0f) ? 0 : 2;
  
  float4 mask = float4((ix + iy) == 0, (ix + iy) == 1, (ix + iy) == 2, (ix + iy) == 3);
  float cornerR = dot(borderRadius, mask);
  
  if (cornerR < 0.01f) {
    // Skip radius check if corner is too small
    return true;
  }
  
  float2 q = abs(p) - (halfSize - cornerR);
  float dist = length(max(q, 0.0f)) + min(max(q.x, q.y), 0.0f) - cornerR;
  
  float unit = fwidth(dist);
  return 1.0f - smoothstep(-unit, 0.0f, dist);
}

float4 main(Input input) : SV_Target0
{
  float alpha = get_rounded_rect_smooth(input.fragmentPosition, input.spriteBounds, input.borderRadius);
  return float4(input.color.xyz, alpha * input.color.a);
}
