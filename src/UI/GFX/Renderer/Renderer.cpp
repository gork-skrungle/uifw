#include "Renderer.hpp"

#include <SDL3/SDL_gpu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "UI/ECS/Components/BaseComponent.hpp"
#include "UI/ECS/Components/RenderingComponents.hpp"
#include "UI/GFX/Renderer/RendererTypes.hpp"
#include "UI/GFX/Shader.hpp"
#include "UI/Window/Window.hpp"
#include "Utils.hpp"

#include <vector>

#ifdef UI_DEBUG_ENABLED
#define UI_VALIDATION_ENABLED true
#else
#define UI_VALIDATION_ENABLED false
#endif

constexpr uint32_t MAX_SPRITE_COUNT = 8192;

using namespace ui;

inline void pick_window_present_mode(const Renderer *renderer)
{
  SDL_GPUPresentMode presentMode = SDL_GPU_PRESENTMODE_VSYNC;

  if (SDL_WindowSupportsGPUPresentMode(renderer->internals.gpuDevice,
                                       renderer->internals.sdlWindowPtr,
                                       SDL_GPU_PRESENTMODE_IMMEDIATE)) {
    presentMode = SDL_GPU_PRESENTMODE_IMMEDIATE;
  } else if (SDL_WindowSupportsGPUPresentMode(renderer->internals.gpuDevice,
                                              renderer->internals.sdlWindowPtr,
                                              SDL_GPU_PRESENTMODE_MAILBOX)) {
    presentMode = SDL_GPU_PRESENTMODE_MAILBOX;
  }

  SDL_SetGPUSwapchainParameters(renderer->internals.gpuDevice,
                                renderer->internals.sdlWindowPtr,
                                SDL_GPU_SWAPCHAINCOMPOSITION_SDR, presentMode);
}

inline std::vector<SpriteInstance> record_draw_list(const Canvas *canvas)
{
  const auto world = canvas->entity.world();
  auto query = world.query<ecs::BaseComponent, ecs::QuadRenderer>();

  std::vector<SpriteInstance> instanceList(query.count());
  size_t counter = 0;

  if (instanceList.empty()) {
    return instanceList;
  }

  query.each([&instanceList, &counter](ecs::Entity e,
                                       ecs::BaseComponent &baseComponent,
                                       ecs::QuadRenderer &quadRenderer) {
    instanceList[counter] = {
        .position = {static_cast<float>(baseComponent.rect.x),
                     static_cast<float>(baseComponent.rect.y),
                     static_cast<float>(baseComponent.zOrder)},
        .rotation = 0.0f,
        .size =
            {
                static_cast<float>(baseComponent.rect.width),
                static_cast<float>(baseComponent.rect.height),
            },
        .color =
            {
                quadRenderer.color.r,
                quadRenderer.color.g,
                quadRenderer.color.b,
                quadRenderer.color.a,
            },
    };

    counter++;
  });

  return instanceList;
}

inline DrawPipeline create_draw_pipeline(const Renderer *renderer,
                                         const Canvas *canvas)
{
  DrawPipeline pipeline = {};

  auto *gpuDevice = renderer->internals.gpuDevice;
  auto *window = renderer->internals.sdlWindowPtr;

  // Load shaders
  constexpr auto VS_PATH = "res/shaders/_compiled/SPIRV/batch_render.vert.spv";
  constexpr auto FS_PATH = "res/shaders/_compiled/SPIRV/batch_render.frag.spv";

  SDL_GPUShader *vertexShader =
      createShader(renderer, VS_PATH, ShaderStage_Vertex, 0, 1, 1, 0);
  SDL_GPUShader *fragmentShader =
      createShader(renderer, FS_PATH, ShaderStage_Fragment, 0, 0, 0, 0);

  // Create sprite render pipeline
  const SDL_GPUColorTargetDescription colorDesc = {
      .format = SDL_GetGPUSwapchainTextureFormat(gpuDevice, window),
      .blend_state = {
          .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
          .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
          .color_blend_op = SDL_GPU_BLENDOP_ADD,
          .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
          .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
          .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
          .enable_blend = true}};

  const SDL_GPUGraphicsPipelineTargetInfo targetInfo = {
      .color_target_descriptions = &colorDesc, .num_color_targets = 1};

  SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
      .vertex_shader = vertexShader,
      .fragment_shader = fragmentShader,
      .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
      .target_info = targetInfo};

  pipeline.renderPipeline =
      SDL_CreateGPUGraphicsPipeline(gpuDevice, &pipelineCreateInfo);

  // Cleanup shaders
  destroyShader(vertexShader, renderer);
  destroyShader(fragmentShader, renderer);

  // Build render pipeline
  const auto drawList = record_draw_list(canvas);

  // Create data transfer buffer
  constexpr SDL_GPUTransferBufferCreateInfo transferBufferInfo = {
      .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
      .size = static_cast<uint32_t>(MAX_SPRITE_COUNT * sizeof(SpriteInstance))};

  pipeline.spriteDataTransferBuffer =
      SDL_CreateGPUTransferBuffer(gpuDevice, &transferBufferInfo);

  // Create data buffer
  constexpr SDL_GPUBufferCreateInfo bufferCreateInfo = {
      .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
      .size = static_cast<uint32_t>(MAX_SPRITE_COUNT * sizeof(SpriteInstance))};

  pipeline.spriteDataBuffer = SDL_CreateGPUBuffer(gpuDevice, &bufferCreateInfo);

  // Record size for rendering
  pipeline.drawListSize = drawList.size();

  // Transfer up-front data (not needed currently, no textures)
  // ...

  return pipeline;
}

Renderer ui::createRenderer(const Window *window, const Canvas *canvas)
{
  Renderer renderer;

  renderer.internals.sdlWindowPtr = window->ptr;
  renderer.drawPipeline.drawListSize = 0;

  // Create GPU device
  SDL_Log("Creating GPU device...\n");

  renderer.internals.gpuDevice = SDL_CreateGPUDevice(
      SDL_GPU_SHADERFORMAT_SPIRV, UI_VALIDATION_ENABLED, nullptr);

  UI_LOG_MSG("Created GPU device (Backend: %s)",
             SDL_GetGPUDeviceDriver(renderer.internals.gpuDevice));

  if (!SDL_ClaimWindowForGPUDevice(renderer.internals.gpuDevice,
                                   renderer.internals.sdlWindowPtr)) {
    UI_LOG_MSG("Failed to claim GPU device");
    return renderer;
  }

  pick_window_present_mode(&renderer);

  renderer.drawPipeline = create_draw_pipeline(&renderer, canvas);

  return renderer;
}

void ui::draw(const Window *window)
{
  // Setup camera matrix
  const Rect windowBounds = getWindowBounds(window);

  const glm::mat4 cameraMatrix = glm::orthoZO(
    0.0f,
    static_cast<float>(windowBounds.width),
    static_cast<float>(windowBounds.height),
    0.0f, -1000.0f, 1000.0f
  );

  // Get command buffer
  auto *gpuDevice = window->renderer.internals.gpuDevice;
  auto *windowPtr = window->renderer.internals.sdlWindowPtr;

  SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(gpuDevice);

  if (cmd == nullptr) {
    UI_LOG_MSG("Failed to acquire GPUCommandBuffer: %s", SDL_GetError());
    return;
  }

  SDL_GPUTexture *swapchainTexture;

  if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmd, windowPtr, &swapchainTexture,
                                             nullptr, nullptr)) {
    UI_LOG_MSG("Failed to acquire GPUSwapchainTexture: %s", SDL_GetError());
    return;
  }

  if (swapchainTexture != nullptr) {
    // Transfer sprite instance data
    auto &drawPipeline = window->renderer.drawPipeline;

    const auto drawList = record_draw_list(&window->canvas);

    const auto dataPtr = static_cast<SpriteInstance *>(SDL_MapGPUTransferBuffer(
        gpuDevice, drawPipeline.spriteDataTransferBuffer, true));

    for (uint32_t i = 0; i < drawList.size(); i++) {
      dataPtr[i] = drawList[i];
    }

    SDL_UnmapGPUTransferBuffer(gpuDevice,
                               drawPipeline.spriteDataTransferBuffer);

    // Upload instance data
    SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmd);

    const SDL_GPUTransferBufferLocation transferBufferLocation = {
        .transfer_buffer = drawPipeline.spriteDataTransferBuffer, .offset = 0};

    const SDL_GPUBufferRegion bufferRegion = {
        .buffer = drawPipeline.spriteDataBuffer,
        .offset = 0,
        .size =
            static_cast<uint32_t>(drawList.size() * sizeof(SpriteInstance))};

    SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion,
                          true);

    SDL_EndGPUCopyPass(copyPass);

    // Render everything
    const SDL_GPUColorTargetInfo colorTargetInfo = {
        .texture = swapchainTexture,
        .clear_color = {0.0f, 0.0f, 0.0f, 1.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE,
        .cycle = false,
    };

    SDL_GPURenderPass *renderPass =
        SDL_BeginGPURenderPass(cmd, &colorTargetInfo, 1, nullptr);

    SDL_BindGPUGraphicsPipeline(renderPass, drawPipeline.renderPipeline);
    SDL_BindGPUVertexStorageBuffers(renderPass, 0,
                                    &drawPipeline.spriteDataBuffer, 1);

    SDL_PushGPUVertexUniformData(cmd, 0, &cameraMatrix, sizeof(glm::mat4));
    SDL_DrawGPUPrimitives(renderPass, drawList.size() * 6, 1, 0, 0);

    SDL_EndGPURenderPass(renderPass);
  }

  SDL_SubmitGPUCommandBuffer(cmd);
}
