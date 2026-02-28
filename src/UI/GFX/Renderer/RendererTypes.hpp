#pragma once

#include "Utils.hpp"

namespace ui {

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

}