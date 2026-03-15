#include "RendererHelpers.h"

void ui_Renderer_pickWindowPresentMode(const ui_Renderer *renderer)
{
  SDL_GPUPresentMode presentMode = SDL_GPU_PRESENTMODE_VSYNC;

  if (SDL_WindowSupportsGPUPresentMode(renderer->gpu_device, renderer->window_ref,
                                       SDL_GPU_PRESENTMODE_IMMEDIATE)) {
    presentMode = SDL_GPU_PRESENTMODE_IMMEDIATE;
  }
  else if (SDL_WindowSupportsGPUPresentMode(renderer->gpu_device, renderer->window_ref,
                                            SDL_GPU_PRESENTMODE_MAILBOX)) {
    presentMode = SDL_GPU_PRESENTMODE_MAILBOX;
  }

  SDL_SetGPUSwapchainParameters(renderer->gpu_device, renderer->window_ref,
                                SDL_GPU_SWAPCHAINCOMPOSITION_SDR, presentMode);
}
