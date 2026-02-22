#pragma once

#include "UI/ECS/ECSRoot/ECSRoot.hpp"
#include "UI/ECS/Entity/Entity.hpp"

namespace ui {

struct Canvas
{
  ecs::Entity entity;
};

Canvas createCanvas(const ecs::ECSRoot *root,
                    float x,
                    float y,
                    float width,
                    float height,
                    const char *name);

}  // namespace ui
