#include "FileUtils.h"

#include "uifw/Core/Utils/Log.h"

#include <stdio.h>
#include <stdlib.h>

char *ui_IO_readFileAsString(const char *path)
{
  FILE *file = fopen(path, "rb");

  if (!file) {
    ui_LogFatal("Unable to open file: %s", path);
    return nullptr;
  }

  // Seek to end to get file size
  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return nullptr;
  }

  const long length = ftell(file);

  if (length < 0) {
    fclose(file);
    return nullptr;
  }

  rewind(file);

  // Allocate buffer
  char *buffer = malloc(length + 1);

  if (!buffer) {
    free(buffer);
    fclose(file);
    return nullptr;
  }

  // Read file contents into buffer
  const size_t readSize = fread(buffer, 1, length, file);

  if (readSize != (size_t)length) {
    free(buffer);
    fclose(file);
    return nullptr;
  }

  // End w/ null-terminator
  buffer[length] = '\0';
  fclose(file);

  return buffer;
}

const char *ui_IO_getFileExtension(const char *path)
{
  const char *dotPosition = strrchr(path, '.');

  if (dotPosition && dotPosition != path) {
    return dotPosition;
  }

  return nullptr;
}
