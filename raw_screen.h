#include "includes.h"

#ifndef RAW_SCREEN_H_
#define RAW_SCREEN_H_

/**
 * Represents the "physical screen"
 */
typedef struct
{
    uint8_t _current_page;
    uint8_t _current_col;
} RawScreen;

void draw_from_pblock_bitmap(RawScreen *raw_screen, uint8_t pblock_x,
                             uint8_t pblock_y, PblockBitmap *pblock_bitmap);

#endif /* RAW_SCREEN_H_ */
