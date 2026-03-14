#pragma once

#include "uifw/Core/Coordinates.h"
#include "uifw/Core/Utils/Memory.h"

typedef struct
{
  ui_Vector3f position;
  ui_Color4f color;
} ui_Renderer_Vertex;

typedef struct
{
  ui_Vector3f position;
  float rotation;
  ui_Vector2f size;
  UI_PAD_FLOAT(2);
  ui_Color4f color;
} ui_Renderer_SpriteInstance;

typedef struct
{
  ui_Vector3f position;
  UI_PAD_FLOAT(1);
  ui_Vector2f size;
  UI_PAD_FLOAT(2);
  ui_Vector4f textureCoords;
  ui_Color4f color;
} ui_Renderer_FontGlyphInstance;