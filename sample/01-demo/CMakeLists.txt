add_executable(01-demo
  main.c
)

target_link_libraries(01-demo PUBLIC
  uifw
)

# Compile shaders using SDL_shadercross (user install required)
add_custom_command(
  TARGET 01-demo
  POST_BUILD
  COMMAND bash ${CMAKE_SOURCE_DIR}/res/shaders/compile_shaders.sh
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/res/shaders
  COMMENT "Compiling Shaders..."
)

# Generate fonts with bundled msdf-atlas-gen
add_custom_command(
  TARGET 01-demo
  POST_BUILD
  COMMAND bash ${CMAKE_SOURCE_DIR}/res/fonts/generate_msdf_font_atlases.sh
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/res/fonts
  COMMENT "Generating MSDF font atlases..."
)

# Copy res/ directory
add_custom_command(
  TARGET 01-demo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/res"
  "$<TARGET_FILE_DIR:01-demo>/res"
  COMMENT "Copying assets to build directory..."
  VERBATIM
)