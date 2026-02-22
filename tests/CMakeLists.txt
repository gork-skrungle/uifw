add_executable(uifw-all-tests
  test_main.cpp
  ECS/CanvasTest.hpp
)

target_link_libraries(uifw-all-tests PRIVATE
  uifw
  doctest
)