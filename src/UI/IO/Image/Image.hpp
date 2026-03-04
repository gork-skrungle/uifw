#pragma once

#include <SDL3/SDL.h>

namespace ui {

enum ImageFormat
{
  ImageFormat_Unsupported,
  ImageFormat_PNG,
};

class Image
{
public:
  [[nodiscard]] static SDL_Surface *loadImageFromPath(const char *path,
                                                      SDL_PixelFormat pixelFormat);

private:
  static ImageFormat get_image_format_from_path(const char *path);
};

} // namespace ui
