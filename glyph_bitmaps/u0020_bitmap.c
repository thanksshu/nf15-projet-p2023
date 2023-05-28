/*
 * As U+0020 Space have 0 width, but needed in rendering, we create it here
 */
#include "../includes.h"

const Color u0020_color[12][3] = { 1 };
const Alpha u0020_alpha[12][3] = { 1 };
const Bitmap u0020_bitmap = { .width = 2, .height = GLYPH_HEIGHT,
                              .color = (Color*) u0020_color, .alpha =
                                      (Alpha*) u0020_alpha };
