#pragma once

#include "Input.hpp"
#include "UI/ECS/Components/InputComponents.hpp"
#include "UI/Layout/LayoutTypes.hpp"

namespace ui {

class InputHelpers
{
public:
  static void initSystemCursors();
  static void processEvents(const Window *window);
  static void cleanupSystemCursors();

private:
  static bool is_mouse_in_rect_component(const Vector2i &mousePos, const Rect &rect);
  static void process_cursor_update(const CursorShape &cursorShape);
};

} // namespace ui
