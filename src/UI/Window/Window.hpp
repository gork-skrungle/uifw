#pragma once

#include "UI/Canvas/Canvas.hpp"

#include <SDL3/SDL.h>

#include "UI/Layout/LayoutTypes.hpp"

namespace ui {

struct Shader;

struct Window
{
  SDL_Window *ptr = nullptr;
  SDL_GPUDevice *gpuDevice = nullptr;
  Canvas canvas;
  ecs::ECSRoot ecsRoot;
};

struct DrawPipeline
{
  SDL_GPUBuffer *vertexBuffer = nullptr;
  SDL_GPUTransferBuffer *transferBuffer = nullptr;
  SDL_GPUGraphicsPipeline *graphicsPipeline = nullptr;
};

struct Vertex
{
  float x, y, z;
  float r, g, b, a;
};

static Vertex g_vertices[]{{0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
                           {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},
                           {0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}};

void initPlatform();

[[nodiscard]] DrawPipeline createDrawPipeline(const Window &window,
                                              const Shader &shader);

[[nodiscard]] Rect getWindowBounds(const Window *window);

[[nodiscard]] Canvas createCanvasForWindow(const Window &window,
                                           const ecs::ECSRoot *root);

void initializeWindow(const char *title, int width, int height, Window *window);

bool updateWindow(const Window *window, const DrawPipeline &drawPipeline);

}  // namespace ui