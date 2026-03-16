#pragma once

#include "uifw/Core/Types.h"
#include "uifw/GFX/Renderer/RendererTypes.h"

void ui_Renderer_pickWindowPresentMode(const ui_Renderer *renderer);

void ui_Renderer_createSpriteDrawPipeline(ui_Renderer *renderer, uint32_t maxInstances);
void ui_Renderer_rebuildSpriteDrawList(ui_Renderer *renderer,
                                       ui_ECS_World *world,
                                       SDL_GPUCommandBuffer *cmd);
