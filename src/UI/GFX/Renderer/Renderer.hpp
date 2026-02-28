#pragma once

#include "Utils.hpp"

#include <SDL3/SDL.h>

namespace ui {

UI_FORWARD_DECLARE_STRUCT(Canvas);
UI_FORWARD_DECLARE_STRUCT(Window);

struct SDLInternals
{
  SDL_Window* sdlWindowPtr = nullptr;
  SDL_GPUDevice *gpuDevice = nullptr;
};

struct DrawPipeline
{
  SDL_GPUGraphicsPipeline *renderPipeline = nullptr;
  SDL_GPUTransferBuffer *spriteDataTransferBuffer = nullptr;
  SDL_GPUBuffer *spriteDataBuffer = nullptr;
  size_t drawListSize;
};

struct Renderer
{
  SDLInternals internals;
  DrawPipeline drawPipeline;
};

Renderer createRenderer(const Window* window, const Canvas *canvas);

void draw(const Window *window);

}  // namespace ui
