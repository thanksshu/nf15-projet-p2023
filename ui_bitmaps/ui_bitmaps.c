#include "../includes.h"

const Color test_color[3][3] = { { 0, 1, 0 }, { 1, 0, 1 }, { 0, 1, 0 } };
const Alpha test_alpha[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
const Bitmap test_bitmap = { .height = 3, .width = 3, .color =
                                     (Color*) test_color,
                             .alpha = (Alpha*) test_alpha };
