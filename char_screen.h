#include "includes.h"

#ifndef CHAR_SCREEN_H_
#define CHAR_SCREEN_H_

/**
 * Screen with character blocks
 */
typedef struct
{
    uint64_t _content[BLOCK_COUNT_X][BLOCK_COUNT_Y];
    bool _is_mod[BLOCK_COUNT_X][BLOCK_COUNT_Y];
} CharScreen;

/**
 * CharScreen factory method
 */
CharScreen new_char_screen();

/**
 * Set given block of CharScreen to a character
 */
void set_block(CharScreen *char_screen, uint8_t x, uint8_t y,
               uint64_t character);

/**
 * Set all blocks of CharScreen to "empty"
 */
void clear_content(CharScreen *char_screen);

/**
 * Set all blocks' "is modified" status of CharScreen to false
 */
void _clear_diff(CharScreen *char_screen);

/**
 * Pump CharScreen to RawScreen
 */
void draw(CharScreen *char_screen, RawScreen *raw_screen);

/**
 * Check if a page block need to be modified
 */
bool _need_mod(CharScreen *char_screen, uint8_t pblock_x, uint8_t pblock_y);

/**
 * Generate bitmap for a page block
 */
void _gen_pblock_bitmap(CharScreen *char_screen, uint8_t pblock_x,
                        uint8_t pblock_y, PblockBitmap *pblock_bitmap);

#endif
