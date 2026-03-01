add_subdirectory(doctest)
add_subdirectory(flecs)
add_subdirectory(glm)
add_subdirectory(json)

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL3)