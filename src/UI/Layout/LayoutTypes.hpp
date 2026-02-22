#pragma once

namespace ui {

enum LayoutType
{
  LayoutType_None,
  LayoutType_Horizontal,
  LayoutType_Vertical
};

struct LayoutPadding
{
  float top;
  float right;
  float bottom;
  float left;
};

struct Rect
{
  float x;
  float y;
  float width;
  float height;
};

struct LayoutComponent
{
  LayoutType type;
  LayoutPadding margins;
  LayoutPadding padding;
};

}