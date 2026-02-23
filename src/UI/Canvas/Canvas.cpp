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
      .top = 0.0f,
      .right = 0.0f,
      .bottom = 0.0f,
      .left = 0.0f,
    },
    .spacing = 0.0f,
    .minWidth = 0.0f,
    .minHeight = 0.0f,
    .maxWidth = std::numeric_limits<float>::max(),
    .maxHeight = std::numeric_limits<float>::max(),
  });

  return Canvas{entity};
}