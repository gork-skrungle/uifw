#pragma once

#include "UI/Layout/LayoutTypes.hpp"
#include "UI/ECS/Entity/Entity.hpp"

namespace ui::ecs {

struct TransformRelationship
{
  size_t nChildren = 0;
  Entity first;
  Entity prev;
  Entity next;
  Entity parent;
};

struct BaseComponent {
  TransformRelationship transformRel = {};

  Rect rect = { 0.0f, 0.0f, 128.0f, 128.0f };
  float minWidth = 0.0f;
  float minHeight = 0.0f;
  float maxWidth = std::numeric_limits<float>::max();
  float maxHeight = std::numeric_limits<float>::max();

  float zOrder = 0.0f;
  bool inLayout = false;
  bool needsUpdate = true;
};

}