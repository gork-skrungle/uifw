#include "UI/ECS/Components/RenderingComponents.hpp"
#include "UI/GFX/Shader.hpp"
#include "UI/Window/Window.hpp"

int main()
{
  ui::initPlatform();

  ui::Window window;
  ui::initializeWindow("SDL Window", 1280, 720, &window);

  const auto e1 =
      ui::ecs::createEntity(&window.ecsRoot, 0, 0, 50, 50, "Entity1");
  const auto e2 =
      ui::ecs::createEntity(&window.ecsRoot, 0, 0, 50, 50, "Entity2");
  const auto e3 =
      ui::ecs::createEntity(&window.ecsRoot, 0, 0, 50, 50, "Entity3");

  e1.add<ui::ecs::QuadRenderer>();
  e2.add<ui::ecs::QuadRenderer>();
  e3.add<ui::ecs::QuadRenderer>();

  while (true) {
    if (!ui::updateWindow(&window)) {
      break;
    }
  }

  return EXIT_SUCCESS;
}
