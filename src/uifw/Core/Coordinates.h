#pragma once
#include <stdint.h>

/* ---- Integer vector types ---- */

typedef struct
{
  uint16_t x;
  uint16_t y;
} ui_Vector2i;

typedef struct
{
  uint16_t x;
  uint16_t y;
  uint16_t z;
} ui_Vector3i;

typedef struct
{
  uint16_t x;
  uint16_t y;
  uint16_t z;
  uint16_t w;
} ui_Vector4i;

/* ------------------------------ */

/* ---- Floating point vector types ---- */

typedef struct
{
  float x;
  float y;
} ui_Vector2f;

typedef struct
{
  float x;
  float y;
  float z;
} ui_Vector3f;

typedef struct
{
  float x;
  float y;
  float z;
  float w;
} ui_Vector4f;

typedef struct
{
  float r;
  float g;
  float b;
  float a;
} ui_Color4f;

/* ------------------------------ */

/**
 * Rect type containing x position, y position, width, and height
 */
typedef struct
{
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
} ui_Rect;

typedef enum
{
  LayoutType_Vertical,
  LayoutType_Horizontal,
} ui_LayoutType;

typedef struct
{
  uint16_t top;
  uint16_t right;
  uint16_t bottom;
  uint16_t left;
} ui_LayoutPadding;