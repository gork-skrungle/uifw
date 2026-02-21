add_executable(uifw-all-tests
  ECS/ECSTest.cpp
)

target_link_libraries(uifw-all-tests PRIVATE
  uifw
  doctest
)