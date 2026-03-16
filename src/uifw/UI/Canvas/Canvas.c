#include "Canvas.h"

#include "uifw/ECS/Components.h"
#include "uifw/Platform/Window.h"

ui_ECS_Entity ui_ECS_createCanvas(ui_LayoutType layoutType,
                                  ui_LayoutPadding margins,
                                  uint16_t spacing,
                                  const ui_Window *window)
{
  ui_ECS_World *world = window->scene.world;
  ECS_COMPONENT(world, ui_ECS_LayoutComponent);

  uint16_t windowWidth, windowHeight;
  ui_getWindowSize(window, &windowWidth, &windowHeight);

  const ui_Rect rect = {
    0,
    0,
    windowWidth,
    windowHeight,
  };

  const ui_ECS_Entity entity = ui_ECS_createEntity(world, rect, "Canvas", UI_NULL_ENTITY);

  ecs_set(world, entity, ui_ECS_LayoutComponent, {
    .type = layoutType,
    .margins = margins,
    .spacing = spacing,
  });

  return entity;
}

void ui_ECS_resizeCanvasToWindowSize(const ui_Window *window)
{
  ui_ECS_World *world = window->scene.world;
  const ui_ECS_Entity canvas = window->scene.root_canvas;

  uint16_t windowWidth, windowHeight;
  ui_getWindowSize(window, &windowWidth, &windowHeight);

  ECS_COMPONENT(world, ui_ECS_BaseComponent);

  const auto baseComponent = ecs_get_mut(world, canvas, ui_ECS_BaseComponent);
  baseComponent->coords.w = windowWidth;
  baseComponent->coords.h = windowHeight;
}
