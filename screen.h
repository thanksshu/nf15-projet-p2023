/*
 * screen.h
 *
 * Abstract screen utilities
 *
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "includes.h"

/**
 * CarryCounter
 *
 * Two-digit counter
 */

typedef struct
{
    uint8_t carry_when;
    uint8_t msb;
    uint8_t lsb;
} CarryCounter;

CarryCounter new_carry_counter(int carry_when, int initial_msb,
                               int initial_lsb);

void count(CarryCounter *counter);

typedef struct
{
    uint64_t content[BLOCK_COUNT_X][BLOCK_COUNT_Y];
    bool diff[BLOCK_COUNT_X][BLOCK_COUNT_Y];
} CharScreen;

CharScreen new_char_screen();

void set_block(CharScreen *char_screen, uint8_t x, uint8_t y,
               uint64_t character);

void clear_content(CharScreen *char_screen);

void clear_diff(CharScreen *char_screen);

void draw(CharScreen *char_screen);

bool need_mod(CharScreen *char_screen, uint8_t pblock_x, uint8_t pblock_y);

void gen_pblock_bitmap(CharScreen *char_screen, uint8_t pblock_x,
                       uint8_t pblock_y,
                       PblockBitmap *pblock_bitmap);

#endif
