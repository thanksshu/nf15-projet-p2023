#include "includes.h"

#ifndef CHAR_SCREEN_H_
#define CHAR_SCREEN_H_

/**
 * Half side of the full width bitmap, cause full width bitmap won't fit
 */
typedef enum
{
    Left, Right
} Side;

/**
 * Info of a char_block
 */
typedef struct
{
    Unicode character;
    unint

    next_char;
    Side side;
    Side next_side;
    bool need_mod;
} CharInfo;

/**
 * Screen with char blocks
 */
typedef struct
{
    CharInfo _char_info[BLOCK_COUNT_X][BLOCK_COUNT_Y];
} CharScreen;

/**
 * CharScreen factory method
 */
CharScreen new_char_screen();

/**
 * Set given char block of CharScreen to a character
 */
void set_char_block(CharScreen *char_screen, uint8_t x, uint8_t y,
                    uint64_t character, Side side);

/**
 * Set all char blocks of CharScreen to "empty"
 */
void clear_char_screen(CharScreen *char_screen);

/**
 * Set all char blocks "is modified" status to false
 */
void clear_char_screen_diff(CharScreen *char_screen);

/**
 * Pump CharScreen to RawScreen
 */
void draw_char_screen(CharScreen *char_screen,
                      RawScreen *raw_screen);

/**
 * Check if a page block need to be modified
 */
bool is_page_block_need_mod(CharScreen *char_screen,
                             uint8_t page_block_x, uint8_t page_block_y);

/**
 * Generate bitmap for a page block
 */
void gen_page_block_bitmap(CharScreen *char_screen,
                            uint8_t page_block_x, uint8_t page_block_y,
                            PageBlockBitmap *page_block_bitmap);

/**
 * Generate bitmap for a page block's row
 */
uint16_t _gen_page_block_row_bitmap(CharScreen *char_screen,
                                          uint8_t char_block_x,
                                          int16_t char_block_y,
                                          uint8_t char_block_row);

#endif
