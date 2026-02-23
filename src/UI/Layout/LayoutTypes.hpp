#pragma once

namespace ui {

enum LayoutType
{
  LayoutType_None,
  LayoutType_Horizontal,
  LayoutType_Vertical
};

struct LayoutMargins
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
  LayoutMargins margins;
  float spacing;

  float minWidth;
  float minHeight;
  float maxWidth;
  float maxHeight;
};

}