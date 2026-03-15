#pragma once

#include "RendererTypes.h"
#include "uifw/Core/Types.h"

#include <SDL3/SDL.h>

void ui_rendererCreate(ui_Window *window);

void ui_rendererDraw(const ui_Window *window);