#include "Input.h"
#include "uifw/Platform/Window.h"

#include <SDL3/SDL.h>

void ui_resetWindowInputState(ui_Window *window, bool firstFrame)
{
  ui_InputState *inputState = &window->input_state;

  inputState->first_frame = firstFrame;
  inputState->should_quit = false;
  inputState->window_resized = false;
  inputState->mouse_moved = false;
  inputState->mouse_down = false;
}

void ui_pollWindowEvents(ui_Window *window)
{
  ui_resetWindowInputState(window, false);

  ui_InputState *inputState = &window->input_state;
  ui_Renderer *renderer = window->renderer;

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      inputState->should_quit = true;
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      inputState->window_resized = true;
      inputState->window_size.x = (uint16_t)event.window.data1;
      inputState->window_size.y = (uint16_t)event.window.data2;

      renderer->is_dirty = true;
      break;
    case SDL_EVENT_MOUSE_MOTION:
      inputState->mouse_moved = true;
      inputState->mouse_position.x = (uint16_t)event.motion.x;
      inputState->mouse_position.y = (uint16_t)event.motion.y;
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      inputState->mouse_down = true;
      break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      inputState->mouse_down = false;
      break;
    case SDL_EVENT_MOUSE_WHEEL:
      break;
    case SDL_EVENT_KEY_DOWN:
      break;
    case SDL_EVENT_KEY_UP:
      break;
    case SDL_EVENT_WINDOW_MINIMIZED:
      break;
    case SDL_EVENT_WINDOW_MAXIMIZED:
      break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
      break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
      break;
    default:
      break;
    }
  }
}
