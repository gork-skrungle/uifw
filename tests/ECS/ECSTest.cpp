#include <UI/ECS/ECSRoot/ECSRoot.hpp>
#include <UI/ECS/Entity/Entity.hpp>
#include <UI/ECS/Components/BaseComponents.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstring>

using namespace ui;

TEST_CASE("Basic entity")
{
  constexpr float X = 15.0f;
  constexpr float Y = 20.0f;
  constexpr float WIDTH = 250.0f;
  constexpr float HEIGHT = 350.0f;

  const auto NAME = "BasicEntityTest";

  ecs::ECSRoot root;
  const ecs::Entity entity =
      ecs::createEntity(&root, X, Y, WIDTH, HEIGHT, NAME);

  // Get and check data
  const auto component = ecs::getComponent<ecs::BaseComponents>(entity);
  CHECK(component.x == X);
  CHECK(component.y == Y);
  CHECK(component.width == WIDTH);
  CHECK(component.height == HEIGHT);

  // Get w/ pointer & reassign
  constexpr float X_2 = X * 2.0f;
  constexpr float Y_2 = Y * 2.0f;
  constexpr float WIDTH_2 = WIDTH * 2.0f;
  constexpr float HEIGHT_2 = HEIGHT * 2.0f;

  auto mutComponent = ecs::getComponentRef<ecs::BaseComponents>(entity);
  mutComponent.x = X_2;
  mutComponent.y = Y_2;
  mutComponent.width = WIDTH_2;
  mutComponent.height = HEIGHT_2;

  CHECK(mutComponent.x == X_2);
  CHECK(mutComponent.y == Y_2);
  CHECK(mutComponent.width == WIDTH_2);
  CHECK(mutComponent.height == HEIGHT_2);

  // Check name
  const char* entityName = ecs::getName(entity);
  CHECK(strcmp(entityName, NAME) == 0);
}

