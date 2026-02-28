#pragma once

namespace ui::ecs {

struct Color
{
  float r = 1.0f;
  float g = 1.0f;
  float b = 1.0f;
  float a = 1.0f;
};

struct QuadRenderer
{
  Color color;
};

}