#include "Canvas.hpp"

#include "UI/ECS/Components/BaseComponent.hpp"
#include "UI/Layout/LayoutTypes.hpp"

using namespace ui;

Canvas ui::createCanvas(const ecs::ECSRoot *root,
                        const float x,
                        const float y,
                        const float width,
                        const float height,
                        const char *name)
{
  const ecs::Entity entity = ecs::createEntity(root, x, y, width, height, name);

  entity.set<LayoutComponent>({
    .type = LayoutType_Vertical,
    .margins = {
      .top = 0,
      .right = 0,
      .bottom = 0,
      .left = 0,
    },
    .padding = {
      .top = 0,
      .right = 0,
      .bottom = 0,
      .left = 0,
    }
  });

  return Canvas{entity};
}