#pragma once

#include <iostream>
#include <cstdarg>
#include <cstdio>

// Struct forward declaration
#define UI_FORWARD_DECLARE_STRUCT(n) struct n

/* ---- Struct padding ---- */
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define UI_PAD_BYTES(size) char TOKENPASTE2(padding_, __LINE__)[size]
#define UI_PAD_FLOAT(size) float TOKENPASTE2(padding_, __LINE__)[size]
/* ------------------------ */

/* ---- Debug flag ---- */
#ifdef _NDEBUG
#undef UI_DEBUG_ENABLED
#else
#define UI_DEBUG_ENABLED 1
#endif
/* -------------------- */

/* ---- Logging ---- */
namespace ui {

inline void logMessage(const char *file,
                       const int line,
                       const char *func,
                       const char *fmt,
                       ...)
{
  std::fprintf(stdout, "[FILE: %s, LINE: %d, FUNC: %s] : ",
    file, line, func);

  va_list args;
  va_start(args, fmt);
  std::vfprintf(stdout, fmt, args);
  va_end(args);

  std::fprintf(stdout, "\n");
}

}

#ifdef UI_DEBUG_ENABLED
#define UI_LOG_MSG(fmt, ...) \
  ui::logMessage(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#else
#define UI_LOG_MSG(fmt, ...)
#endif
/* ----------------- */

