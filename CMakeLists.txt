cmake_minimum_required(VERSION 4.0)
project(sdl3-ui-test)

add_subdirectory(extern)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_library(uifw STATIC
  src/UI/Window/Window.cpp
  src/UI/Window/Window.hpp
  src/Utils.hpp
  src/UI/GFX/Shader.hpp
  src/UI/GFX/Shader.cpp
  src/UI/Canvas/Canvas.cpp
  src/UI/Canvas/Canvas.hpp
  src/UI/ECS/ECSRoot/ECSRoot.cpp
  src/UI/ECS/ECSRoot/ECSRoot.hpp
  src/UI/ECS/Entity/Entity.cpp
  src/UI/ECS/Entity/Entity.hpp
  src/UI/ECS/Components/BaseComponent.hpp
  src/UI/Layout/LayoutHelpers.cpp
  src/UI/Layout/LayoutHelpers.hpp
)

target_link_libraries(uifw PUBLIC
  flecs
  SDL3::SDL3-static
  glm::glm
)

target_include_directories(uifw PUBLIC
  src
)

add_executable(sdl3-ui-test
  main.cpp
  src/UI/Layout/LayoutTypes.hpp
  src/UI/Layout/LayoutHelpers.cpp
  src/UI/Layout/LayoutHelpers.hpp
  src/UI/ECS/Components/RenderingComponents.hpp
  src/UI/GFX/Renderer/Renderer.cpp
  src/UI/GFX/Renderer/Renderer.hpp
  src/UI/GFX/Renderer/RendererTypes.hpp
)

target_link_libraries(sdl3-ui-test PRIVATE
  uifw
)

add_subdirectory(tests)

# Copy res/ directory
add_custom_command(
  TARGET sdl3-ui-test
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/res"
    "$<TARGET_FILE_DIR:sdl3-ui-test>/res"
  COMMENT "\n\n => Copying assets to build directory...\nDone.\n"
  VERBATIM
)
