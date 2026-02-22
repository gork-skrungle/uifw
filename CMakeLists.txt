cmake_minimum_required(VERSION 3.5)
project(sdl3-ui-test)

add_subdirectory(extern)

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
)

target_link_libraries(uifw PUBLIC
  flecs
  SDL3::SDL3-static
)

target_include_directories(uifw PUBLIC
  src
)

add_executable(sdl3-ui-test
  main.cpp
  src/UI/Layout/LayoutTypes.hpp
)

target_link_libraries(sdl3-ui-test PRIVATE
  uifw
)

add_subdirectory(tests)