#pragma once

#include "UI/ECS/ECSRoot/ECSRoot.hpp"

#include <flecs.h>

namespace ui::ecs {

typedef flecs::entity Entity;

Entity createEntity(const ECSRoot *root,
                    float x,
                    float y,
                    float width = 128.0f,
                    float height = 128.0f,
                    const char *name = "");

}  // namespace ui::ecs
