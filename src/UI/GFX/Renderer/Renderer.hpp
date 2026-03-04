#pragma once

#include "RendererTypes.hpp"
#include "UI/ECS/Components/BaseComponent.hpp"
#include "UI/ECS/Components/FontComponents.hpp"
#include "UI/ECS/Entity/Entity.hpp"
#include "Utils.hpp"

#include <SDL3/SDL.h>

#include <vector>

namespace ui {

UI_FORWARD_DECLARE_STRUCT(Canvas);
UI_FORWARD_DECLARE_STRUCT(Window);

struct SDLInternals
{
  SDL_Window *sdlWindowPtr = nullptr;
  SDL_GPUDevice *gpuDevice = nullptr;
};

struct DrawPipeline
{
  // Sprites
  SDL_GPUGraphicsPipeline *spriteDataPipeline = nullptr;
  SDL_GPUTransferBuffer *spriteDataTransferBuffer = nullptr;
  SDL_GPUBuffer *spriteDataBuffer = nullptr;
  size_t spriteDrawListSize = 0;
  // Text
  SDL_GPUGraphicsPipeline *textPipeline = nullptr;
  SDL_GPUSampler *fontAtlasSampler = nullptr;
  SDL_GPUTexture *fontAtlasTexture = nullptr;
  SDL_GPUTransferBuffer *textTransferBuffer = nullptr;
  SDL_GPUBuffer *textBuffer = nullptr;
  size_t textDrawListSize = 0;
};

struct RendererData
{
  SDLInternals internals;
  DrawPipeline drawPipeline;
};

class Renderer
{
public:
  static RendererData createRenderer(const Window *window, const Canvas *canvas);

  static void draw(const Window *window);

private:
  static void pick_window_present_mode(const RendererData *renderer);

  static DrawPipeline create_draw_pipeline(const RendererData *renderer,
                                           const Canvas *canvas);

  static std::vector<SpriteInstance> record_sprite_draw_list(const Canvas *canvas);

  static std::vector<FontGlyphInstance> record_glyph_draw_list(const Canvas *canvas);

  static size_t get_text_render_instance_count_from_query(
    const flecs::query<ecs::BaseComponent, TextComponent> &query);

  static void create_text_render_pipeline(const RendererData *renderer,
                                          const Canvas *canvas,
                                          DrawPipeline *pipeline);
};

} // namespace ui
