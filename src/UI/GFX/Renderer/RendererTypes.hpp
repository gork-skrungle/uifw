#pragma once

#include "Utils.hpp"

#include <cstdint>

namespace ui {

struct Vector2i
{
  uint16_t x;
  uint16_t y;
};

struct Vector2f
{
  float x;
  float y;
};

struct Vector3f
{
  float x;
  float y;
  float z;
};

struct Vector4f
{
  float x;
  float y;
  float z;
  float w;
};

struct Color4f
{
  float r;
  float g;
  float b;
  float a;
};

struct Vertex
{
  Vector3f position;
  Color4f color;
};

struct SpriteInstance
{
  Vector3f position;
  float rotation;
  Vector2f size;
  UI_PAD_FLOAT(2);
  Color4f color;
};

struct FontGlyphInstance
{
  Vector3f position;
  UI_PAD_FLOAT(1);
  Vector2f size;
  UI_PAD_FLOAT(2);
  Vector4f textureCoords;
  Color4f color;
};

}