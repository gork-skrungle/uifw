#include "Entity.hpp"

ui::ecs::Entity ui::ecs::createEntity(const ECSRoot *root,
                                      const float x,
                                      const float y,
                                      const float width,
                                      const float height,
                                      const char *name)
{
  const flecs::entity entity = root->world.entity(name);
  entity.set<BaseComponents>({x, y, width, height});

  return Entity{entity};
}

const char *ui::ecs::getName(const Entity entity)
{
  return entity.entity.name().c_str();
}