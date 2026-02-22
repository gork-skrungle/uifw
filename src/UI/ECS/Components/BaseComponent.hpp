#pragma once

#include "UI/Layout/LayoutTypes.hpp"

namespace ui::ecs {

struct BaseComponent {
  Rect rect;
  float zOrder;
  bool inLayout;
  bool needsUpdate;
};

}