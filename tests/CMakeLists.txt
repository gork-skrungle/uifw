add_executable(uifw-tests-all
  main.c
  Core/ContainersTest.h
)

target_link_libraries(uifw-tests-all PRIVATE cmocka::cmocka uifw)