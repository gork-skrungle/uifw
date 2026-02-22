#pragma once

#include "UI/ECS/ECSRoot/ECSRoot.hpp"
#include "UI/ECS/Entity/Entity.hpp"

namespace ui {

struct Canvas
{
  ecs::Entity entity;
};

Canvas createCanvas(const ecs::ECSRoot *root,
                    const float x,
                    const float y,
                    const float width,
                    const float height,
                    const char *name);

}  // namespace ui
