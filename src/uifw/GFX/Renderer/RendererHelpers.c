#include "RendererHelpers.h"

#include "uifw/ECS/Components.h"
#include "uifw/GFX/Shader.h"

#define SPRITE_VS_PATH "res/shaders/_compiled/SPIRV/batch_render.vert.spv"
#define SPRITE_FS_PATH "res/shaders/_compiled/SPIRV/batch_render.frag.spv"

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

void ui_Renderer_createSpriteDrawPipeline(ui_Renderer *renderer,
                                          const uint32_t maxInstances)
{
  SDL_GPUDevice *gpuDevice = renderer->gpu_device;
  SDL_Window *sdlWindowRef = renderer->window_ref;

  // Create shaders
  ui_Shader *spriteVertexShader =
    ui_shaderCreate(SPRITE_VS_PATH, ShaderStage_Vertex, 0, 1, 1, 0, renderer);
  ui_Shader *spriteFragmentShader =
    ui_shaderCreate(SPRITE_FS_PATH, ShaderStage_Fragment, 0, 0, 0, 0, renderer);

  // Allocate structure
  ui_Renderer_DrawPipeline *pipeline = malloc(sizeof(ui_Renderer_DrawPipeline));
  pipeline->type = DrawPipelineType_Sprite;
  pipeline->allocated = 0;
  pipeline->capacity = maxInstances;
  pipeline->pipeline = nullptr;
  pipeline->transfer_buffer = nullptr;
  pipeline->data_buffer = nullptr;
  pipeline->texture_sampler = nullptr;
  pipeline->texture = nullptr;
  pipeline->data = malloc(maxInstances * sizeof(ui_Renderer_SpriteInstance));

  // Create SDL GPU objects
  const SDL_GPUColorTargetDescription colorDesc = {
    .format = SDL_GetGPUSwapchainTextureFormat(gpuDevice, sdlWindowRef),
    .blend_state = {.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                    .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                    .color_blend_op = SDL_GPU_BLENDOP_ADD,
                    .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                    .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                    .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
                    .enable_blend = true}};

  const SDL_GPUGraphicsPipelineTargetInfo targetInfo = {
    .color_target_descriptions = &colorDesc, .num_color_targets = 1};

  const SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
    .vertex_shader = spriteVertexShader,
    .fragment_shader = spriteFragmentShader,
    .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    .target_info = targetInfo};

  pipeline->pipeline = SDL_CreateGPUGraphicsPipeline(gpuDevice, &pipelineCreateInfo);

  ui_shaderDestroy(spriteVertexShader, renderer);
  ui_shaderDestroy(spriteFragmentShader, renderer);

  // Create sprite data transfer buffer
  const SDL_GPUTransferBufferCreateInfo transferBufferInfo = {
    .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
    .size = (uint32_t)(maxInstances * sizeof(ui_Renderer_SpriteInstance))};

  pipeline->transfer_buffer = SDL_CreateGPUTransferBuffer(gpuDevice, &transferBufferInfo);

  // Create sprite data buffer
  const SDL_GPUBufferCreateInfo bufferCreateInfo = {
    .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
    .size = (uint32_t)(maxInstances * sizeof(ui_Renderer_SpriteInstance))};

  pipeline->data_buffer = SDL_CreateGPUBuffer(gpuDevice, &bufferCreateInfo);

  // Set pointer in renderer
  renderer->sprite_pipeline = pipeline;
}

void ui_Renderer_rebuildSpriteDrawList(ui_Renderer *renderer,
                                       ui_ECS_World *world,
                                       SDL_GPUCommandBuffer *cmd)
{
  ECS_COMPONENT(world, ui_ECS_BaseComponent);
  ECS_COMPONENT(world, ui_ECS_QuadRendererComponent);

  const size_t pipelineByteCapacity =
    renderer->sprite_pipeline->capacity * sizeof(ui_Renderer_SpriteInstance);

  // Zero out the entire allocated buffer
  memset(renderer->sprite_pipeline->data, 0, pipelineByteCapacity);

  auto outList = (ui_Renderer_SpriteInstance *)renderer->sprite_pipeline->data;

  // Query sprites
  ecs_query_t *spriteQuery =
    ecs_query(world, {.terms = {
      {.id = ecs_id(ui_ECS_BaseComponent)},
      {.id = ecs_id(ui_ECS_QuadRendererComponent)}
  }});

  ecs_iter_t it = ecs_query_iter(world, spriteQuery);
  size_t counter = 0;

  while (ecs_query_next(&it)) {
    auto base = ecs_field(&it, ui_ECS_BaseComponent, 0);
    auto quadRenderer = ecs_field(&it, ui_ECS_QuadRendererComponent, 1);

    ui_Renderer_SpriteInstance *outElem = &outList[counter];
    outElem->position.x = (float)base->coords.x;
    outElem->position.y = (float)base->coords.y;
    outElem->position.z = (float)base->zOrder;
    outElem->rotation = 0.0f; // TODO: Provide actual rotation value
    outElem->size.x = (float)base->coords.w;
    outElem->size.y = (float)base->coords.h;
    outElem->color = quadRenderer->color;

    ++counter;
  }

  ecs_query_fini(spriteQuery);

  // Update pipeline size and mark as clean
  ui_Renderer_DrawPipeline *pipeline = renderer->sprite_pipeline;

  pipeline->allocated = counter;

  // Map transfer buffer
  SDL_GPUDevice *gpuDevice = renderer->gpu_device;

  auto spriteDataPtr = (ui_Renderer_SpriteInstance *)SDL_MapGPUTransferBuffer(
    gpuDevice, renderer->sprite_pipeline->transfer_buffer, true);

  if (pipeline->allocated > 0) {
    SDL_memcpy(spriteDataPtr, pipeline->data, pipelineByteCapacity);
  }

  SDL_UnmapGPUTransferBuffer(gpuDevice, pipeline->transfer_buffer);

  // Upload data to GPU
  SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmd);

  const SDL_GPUTransferBufferLocation location = {
    .transfer_buffer = pipeline->transfer_buffer,
    .offset = 0
  };

  const SDL_GPUBufferRegion region = {
    .buffer = pipeline->data_buffer,
    .offset = 0,
    .size = pipelineByteCapacity
  };

  SDL_UploadToGPUBuffer(copyPass, &location, &region, true);
  SDL_EndGPUCopyPass(copyPass);
}
