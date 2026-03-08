#pragma once

#include "UI/GFX/Renderer/RendererTypes.hpp"

namespace ui::ecs {

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

struct HoverHandlerComponent
{
  CursorShape cursorShape = CursorShape_Pointer;
};

struct ButtonComponent {};

}