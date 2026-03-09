cmake_minimum_required(VERSION 4.0)

add_library(uifw STATIC
  uifw/Core/Application.cpp
  uifw/Core/Application.hpp
)

target_link_libraries(uifw PUBLIC
  flecs
  SDL3::SDL3-static
  glm::glm
  nlohmann_json::nlohmann_json
  stb
)

target_include_directories(uifw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})