#include "Entity.hpp"
#include "UI/ECS/Components/BaseComponents.hpp"

ui::ecs::Entity ui::ecs::createEntity(const ECSRoot *root,
                                      const float x,
                                      const float y,
                                      const float width,
                                      const float height,
                                      const char *name)
{
  const Entity entity = root->world.entity(name);

  entity.set<BaseComponents>({
    .rect = {x, y, width, height},
    .zOrder = 0.0f,
    .inLayout = false,
  });

  return entity;
}