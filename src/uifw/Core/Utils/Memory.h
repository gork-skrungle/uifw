#pragma once

/* ---- STRUCT PADDING ---- */

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define UI_PAD_BYTES(size) char TOKENPASTE2(padding_, __LINE__)[size]
#define UI_PAD_FLOAT(size) float TOKENPASTE2(padding_, __LINE__)[size]

/* ------------------------ */