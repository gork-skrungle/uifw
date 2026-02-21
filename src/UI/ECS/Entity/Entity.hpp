#pragma once

#include "UI/ECS/Components/BaseComponents.hpp"
#include "UI/ECS/ECSRoot/ECSRoot.hpp"

#include "flecs.h"

namespace ui::ecs {

struct Entity
{
  flecs::entity entity;
};

Entity createEntity(const ECSRoot *root,
                    float x,
                    float y,
                    float width = 128.0f,
                    float height = 128.0f,
                    const char *name = "");

template<typename T>
T& addComponent(Entity* entity, T component)
{
  return entity->entity.insert(component);
}

template<typename T>
const T& getComponent(Entity entity)
{
  return entity.entity.get<T>();
}

template<typename T>
T& getComponentRef(Entity entity)
{
  return entity.entity.get_mut<T>();
}

const char *getName(const Entity entity);

}  // namespace ui::ecs
