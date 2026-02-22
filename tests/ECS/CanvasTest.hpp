#pragma once

#include "UI/Canvas/Canvas.hpp"
#include "UI/Window/Window.hpp"

#include <doctest/doctest.h>

#include "UI/ECS/Components/BaseComponent.hpp"

TEST_CASE("Construct window with canvas")
{
  constexpr int WINDOW_WIDTH = 1280;
  constexpr int WINDOW_HEIGHT = 720;

  ui::initPlatform();

  ui::Window window;
  ui::initializeWindow("Test", WINDOW_WIDTH, WINDOW_HEIGHT, &window);

  ui::Canvas& canvas = window.canvas;

  CHECK(canvas.entity.has<ui::LayoutComponent>());
  CHECK(canvas.entity.name() == "Canvas");

  auto baseComponent = canvas.entity.get<ui::ecs::BaseComponent>();

  CHECK(baseComponent.rect.width == WINDOW_WIDTH);
  CHECK(baseComponent.rect.height == WINDOW_HEIGHT);

  auto [type, margins, padding] = canvas.entity.get<ui::LayoutComponent>();

  CHECK(type == ui::LayoutType_Vertical);

  CHECK(margins.top == 0.0f);
  CHECK(margins.right == 0.0f);
  CHECK(margins.bottom == 0.0f);
  CHECK(margins.left == 0.0f);

  CHECK(padding.top == 0.0f);
  CHECK(padding.right == 0.0f);
  CHECK(padding.bottom == 0.0f);
  CHECK(padding.left == 0.0f);
}