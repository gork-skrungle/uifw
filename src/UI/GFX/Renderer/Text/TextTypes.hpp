#pragma once

#include <cstdint>
#include <vector>

#include "UI/GFX/Renderer/RendererTypes.hpp"

namespace ui {

enum FontAtlasType
{
  FontAtlasType_Unknown,
  FontAtlasType_MSDF,
};

enum FontYOrigin
{
  FontYOrigin_Bottom,
  FontYOrigin_Middle,
  FontYOrigin_Top,
};

struct FontAtlas
{
  FontAtlasType type;
  uint16_t distanceRange;
  uint16_t distanceRangeMiddle;
  float size;
  Vector2i atlasDimensions;
  FontYOrigin yOrigin;
};

struct FontMetrics
{
  uint16_t emSize;
  float lineHeight;
  float ascender;
  float descender;
  float underlineY;
  float underlineThickness;
};

struct FontGlyphBounds
{
  float left;
  float bottom;
  float right;
  float top;
};

struct FontGlyph
{
  float advance;
  FontGlyphBounds planeBounds;
  FontGlyphBounds atlasBounds;
};

struct FontData
{
  FontAtlas atlas;
  FontMetrics metrics;
  std::vector<FontGlyph> glyphs;
};

}