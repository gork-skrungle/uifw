#pragma once

#include "uifw/Core/Types.h"

void ui_resetWindowInputState(ui_Window *window, bool firstFrame);

void ui_pollWindowEvents(ui_Window *window);