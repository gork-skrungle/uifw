#pragma once

#include "UI/Layout/LayoutTypes.hpp"

namespace ui::ecs {

struct BaseComponents {
  Rect rect;
  float zOrder;
  bool inLayout;
};

}