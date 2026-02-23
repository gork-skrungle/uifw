#pragma once

#include "UI/ECS/ECSRoot/ECSRoot.hpp"

#include <flecs.h>

#define UI_NULL_ENTITY flecs::entity::null()
#define UI_REF(T) flecs::ref<T>

namespace ui::ecs {

typedef flecs::entity Entity;

Entity createEntity(const ECSRoot *root,
                    float x,
                    float y,
                    float width = 128.0f,
                    float height = 128.0f,
                    const char *name = "",
                    const Entity *parent = nullptr);

}  // namespace ui::ecs
