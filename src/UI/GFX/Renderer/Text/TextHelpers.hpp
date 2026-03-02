#pragma once

#include "UI/ECS/Components/FontComponents.hpp"
#include "UI/ECS/Entity/Entity.hpp"

namespace ui {

class TextHelpers
{
public:
  static ecs::Entity createTextEntity(const ecs::ECSRoot *root,
                                      FontData *fontData,
                                      const char *text,
                                      uint16_t pixelSize,
                                      uint16_t x,
                                      uint16_t y,
                                      uint16_t width = 128.0f,
                                      uint16_t height = 128.0f,
                                      const char *name = "",
                                      const ecs::Entity *parent = nullptr);
};

} // namespace ui
