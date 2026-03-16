#include <stdio.h>

#include "uifw/Core/Containers/Queue.h"
#include "uifw/Core/Utils/Log.h"
#include "uifw/ECS/Components.h"
#include "uifw/uifw.h"

int main()
{
  ui_Application app = ui_initApplication();

  const ui_WindowParams winParams = {
    .title = "uifw window",
    .width = 1280,
    .height = 720,
  };
  ui_Window *window = ui_createWindow(winParams, &app);

  /* ---- BUILD EXAMPLE SCENE ---- */

  ecs_world_t *world = window->scene.world;
  ECS_COMPONENT(world, ui_ECS_QuadRendererComponent);

  ui_Rect quadCoords;
  quadCoords.x = 0;
  quadCoords.y = 50;
  quadCoords.w = 100;
  quadCoords.h = 100;

  const ui_ECS_Entity quad1 =
    ui_ECS_createEntity(world, quadCoords, "quad1", window->scene.root_canvas);
  const ui_ECS_Entity quad2 =
    ui_ECS_createEntity(world, quadCoords, "quad2", window->scene.root_canvas);

  ecs_set(world, quad1, ui_ECS_QuadRendererComponent, {
    .color = {1.0, 0.0, 0.0, 1.0}
  });
  ecs_set(world, quad2, ui_ECS_QuadRendererComponent, {
    .color = {0.0, 1.0, 0.0, 1.0}
  });

  /* ----------------------------- */

  while (ui_updateApplication(&app)) {
    // User event loop
  }

  ui_destroyWindow(window);
}