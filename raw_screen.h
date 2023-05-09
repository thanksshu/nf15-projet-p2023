#include "includes.h"

#ifndef RAW_SCREEN_H_
#define RAW_SCREEN_H_



/**
 * Page block bitmap
 */
typedef BlockRowBitmap PageBlockBitmap[PAGE_HEIGHT];

/**
 * Represents the "physical screen"
 */
typedef struct
{
    uint8_t _current_page;
    uint8_t _current_col;
} RawScreen;

/**
 * Draw a pblock bitmap on to the real screen
 *
 */
void draw_from_page_block_bitmap(RawScreen *raw_screen, uint8_t page_block_x,
                             uint8_t page_block_y, PageBlockBitmap *page_block_bitmap);

/**
 * Generate a column for the real screen
 */
uint8_t _gen_pblock_col_bitmap(PageBlockBitmap *page_block_bitmap, uint8_t bitmap_col);

#endif /* RAW_SCREEN_H_ */
