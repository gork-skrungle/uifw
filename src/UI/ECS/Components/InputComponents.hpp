#pragma once

#include "UI/GFX/Renderer/RendererTypes.hpp"

namespace ui {

enum CursorShape
{
  CursorShape_Default,
  CursorShape_Hide,
  CursorShape_Pointer,
  CursorShape_Grab,
  CursorShape_Move,
  CursorShape_VerticalResize,
  CursorShape_HorizontalResize,
};

namespace ecs {

struct HoverHandlerComponent
{
  CursorShape cursorShape = CursorShape_Pointer;
};

struct ButtonComponent
{};

} // namespace ecs

} // namespace ui
