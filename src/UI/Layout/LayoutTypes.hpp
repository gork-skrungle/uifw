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

struct LayoutParams
{
  LayoutPadding margins;
  LayoutPadding padding;
};

struct Rect
{
  float x;
  float y;
  float width;
  float height;
};

}