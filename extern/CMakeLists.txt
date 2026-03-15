add_subdirectory(cglm)
add_subdirectory(flecs)
add_subdirectory(stb)

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL3)