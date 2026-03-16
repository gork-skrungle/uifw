#include <stdio.h>

#include "uifw/Core/Containers/Queue.h"
#include "uifw/Core/Utils/Log.h"
#include "uifw/ECS/Components.h"
#include "uifw/UI/Layout/Layout.h"
#include "uifw/uifw.h"

#define DEMO_WINDOW_WIDTH 1280
#define DEMO_WINDOW_HEIGHT 720

int main()
{
  ui_Application app = ui_initApplication();

  const ui_WindowParams winParams = {
    .title = "uifw window",
    .width = DEMO_WINDOW_WIDTH,
    .height = DEMO_WINDOW_HEIGHT,
  };
  ui_Window *window = ui_createWindow(winParams, &app);

  /* ---- BUILD DEMO SCENE ---- */

  ecs_world_t *world = window->scene.world;
  ECS_COMPONENT(world, ui_ECS_QuadRendererComponent);
  ECS_COMPONENT(world, ui_ECS_LayoutComponent);
  ECS_COMPONENT(world, ui_ECS_BaseComponent);

  /* Set up root canvas layout */
  const auto canvasLayout = ecs_get_mut(world, window->scene.root_canvas, ui_ECS_LayoutComponent);
  canvasLayout->type = LayoutType_Horizontal;
  canvasLayout->margins = (ui_LayoutPadding){10, 10, 10, 10};
  canvasLayout->spacing = 10;

  /* Create Entity1 - vertical container */
  ui_ECS_Entity e1 = ui_ECS_createEntity(world, (ui_Rect){0, 0, 50, 50}, "Entity1",
                                               window->scene.root_canvas);

  auto e1Base = ecs_get_mut(world, e1, ui_ECS_BaseComponent);
  e1Base->minWidth = 500;
  e1Base->maxWidth = 500;

  ecs_set(world, e1, ui_ECS_LayoutComponent, {
    .type = LayoutType_Vertical,
    .margins = {10, 10, 10, 10},
    .spacing = 0,
  });

  /* Create gradient quads in Entity1 */
  const float gradientStart[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  const float gradientEnd[4] = {0.5f, 0.5f, 0.5f, 1.0f};

  for (uint32_t i = 0; i < 20; ++i) {
    char nameBuffer[32];
    snprintf(nameBuffer, sizeof(nameBuffer), "GradientQuad%u", i);

    const float t = (float)i / 19.0f;
    const ui_Color4f color = {
      .r = gradientStart[0] + (gradientEnd[0] - gradientStart[0]) * t,
      .g = gradientStart[1] + (gradientEnd[1] - gradientStart[1]) * t,
      .b = gradientStart[2] + (gradientEnd[2] - gradientStart[2]) * t,
      .a = gradientStart[3] + (gradientEnd[3] - gradientStart[3]) * t,
    };

    const ui_ECS_Entity quad = ui_ECS_createEntity(world, (ui_Rect){0, 0, 128, 26}, nameBuffer, e1);
    ecs_set(world, quad, ui_ECS_QuadRendererComponent, {.color = color});

    auto quadBase = ecs_get_mut(world, quad, ui_ECS_BaseComponent);
    quadBase->minHeight = 26;
    quadBase->maxHeight = 26;
  }

  /* Create Entity2 */
  ui_ECS_Entity e2 = ui_ECS_createEntity(world, (ui_Rect){0, 0, 50, 50}, "Entity2",
                                               window->scene.root_canvas);

  /* Create Entity3 - vertical container */
  ui_ECS_Entity e3 = ui_ECS_createEntity(world, (ui_Rect){0, 0, 50, 50}, "Entity3",
                                               window->scene.root_canvas);

  ecs_set(world, e3, ui_ECS_LayoutComponent, {
    .type = LayoutType_Vertical,
    .spacing = 10,
  });

  /* Create Entity4 - nested vertical container inside Entity3 */
  ui_ECS_Entity e4 = ui_ECS_createEntity(world, (ui_Rect){0, 0, 50, 50}, "Entity4", e3);

  ecs_set(world, e4, ui_ECS_LayoutComponent, {
    .type = LayoutType_Vertical,
    .margins = {5, 5, 5, 5},
    .spacing = 3,
  });

  /* Create button-like entities inside Entity4 */
  for (size_t i = 0; i < 5; ++i) {
    char nameBuffer[32];
    snprintf(nameBuffer, sizeof(nameBuffer), "ButtonEntity%zu", i + 1);

    const ui_ECS_Entity button =
      ui_ECS_createEntity(world, (ui_Rect){0, 0, 75, 23}, nameBuffer, e4);

    ecs_set(world, button, ui_ECS_QuadRendererComponent, {
      .color = {0.36f, 0.36f, 0.36f, 1.0f},
    });

    auto buttonBase = ecs_get_mut(world, button, ui_ECS_BaseComponent);
    buttonBase->minHeight = 23;
    buttonBase->maxHeight = 23;
    buttonBase->minWidth = 75;
    buttonBase->maxWidth = 75;
  }

  /* Add spacer entity */
  ui_ECS_createEntity(world, (ui_Rect){0, 0, 50, 50}, "Spacer", e4);

  /* Create Entity5 and Entity6 inside Entity3 */
  const ui_ECS_Entity e5 = ui_ECS_createEntity(world, (ui_Rect){0, 0, 50, 50}, "Entity5", e3);
  const ui_ECS_Entity e6 = ui_ECS_createEntity(world, (ui_Rect){0, 0, 50, 50}, "Entity6", e3);

  auto e6Base = ecs_get_mut(world, e6, ui_ECS_BaseComponent);
  e6Base->minHeight = 100;
  e6Base->maxHeight = 100;
  e6Base->minWidth = 100;
  e6Base->maxWidth = 100;

  /* Add QuadRendererComponents to all entities */
  const ui_Color4f red = {0.25f, 0.25f, 0.25f, 1.0f};
  const ui_Color4f green = {0.1f, 0.1f, 0.1f, 1.0f};
  const ui_Color4f blue = {0.75f, 0.75f, 1.75f, 1.0f};
  const ui_Color4f white = {0.5f, 0.5f, 0.5f, 1.0f};

  ecs_set(world, e1, ui_ECS_QuadRendererComponent, {.color = red});
  ecs_set(world, e2, ui_ECS_QuadRendererComponent, {.color = green});
  ecs_set(world, e4, ui_ECS_QuadRendererComponent, {.color = blue});
  ecs_set(world, e5, ui_ECS_QuadRendererComponent, {.color = white});
  ecs_set(world, e6, ui_ECS_QuadRendererComponent, {.color = red});

  while (ui_updateApplication(&app)) {
    // User event loop
  }

  ui_destroyWindow(window);
}