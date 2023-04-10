/*
 * screen.h
 *
 * Abstract screen utilities
 */

#include "includes.h"

CarryCounter new_carry_counter(int carry_when, int initial_msb, int initial_lsb)
{
    CarryCounter counter;
    counter.carry_when = carry_when;
    counter.msb = initial_msb;
    counter.lsb = initial_lsb;
    return counter;
}

void count(CarryCounter *counter)
{
    // increase lsb, then carry
    if (counter->lsb < counter->carry_when - 1)
    {
        counter->lsb += 1;
    }
    else
    {
        counter->msb += 1;
        counter->lsb = 0;
    }
}

CharScreen new_char_screen(){
    CharScreen char_screen;
    clear_content(&char_screen);
    clear_diff(&char_screen);
    return char_screen;
}

void set_block(CharScreen *char_screen, uint8_t x, uint8_t y,
               uint64_t character)
{
    if ((char_screen->content)[x][y] == character)
    {
        return;
    }
    (char_screen->diff)[x][y] = true;
    (char_screen->content)[x][y] = character;
}

void clear_content(CharScreen *char_screen)
{
    uint8_t x = 0;
    uint8_t y = 0;
    for (x = 0; x < BLOCK_COUNT_X; ++x)
    {
        for (y = 0; y < BLOCK_COUNT_Y; ++y)
        {
            set_block(char_screen, x, y, EMPTY_CHAR);
        }
    }
}

void clear_diff(CharScreen *char_screen)
{
    uint8_t x = 0;
    uint8_t y = 0;
    for (x = 0; x < BLOCK_COUNT_X; ++x)
    {
        for (y = 0; y < BLOCK_COUNT_Y; ++y)
        {
            (char_screen->diff)[x][y] = false;
        }
    }
}

void draw(CharScreen *char_screen)
{
    uint8_t pblock_x = 0;
    uint8_t pblock_y = 0;
    PblockBitmap pblock_bitmap = { 0 };
    for (pblock_y = 0; pblock_y < PAGE_COUNT; ++pblock_y)
    {
        for (pblock_x = 0; pblock_x < BLOCK_COUNT_X; ++pblock_x)
        {
            if (need_mod(char_screen, pblock_x, pblock_y))
            {
                gen_pblock_bitmap(char_screen, pblock_x, pblock_y,
                                  &pblock_bitmap);
                // draw_from_page_bitmap(pblock_x, pblock_y, pblock_bitmap)
            }
        }
    }
    clear_diff(char_screen);
}

bool need_mod(CharScreen *char_screen, uint8_t pblock_x, uint8_t pblock_y)
{
    bool mod = false;
    uint8_t cblock_x = pblock_x;
    int8_t cblock_y = 0;

    int8_t first_cblock = P2B_MAP[pblock_y][0];
    int8_t last_cblock = P2B_MAP[pblock_y][1];
    for (cblock_y = first_cblock; cblock_y < last_cblock + 1; ++cblock_y)
    {
        if ((cblock_y < 0) || (cblock_y > BLOCK_COUNT_Y - 1))
        {
            mod = mod || false;
        }
        else
        {
            mod = mod || (char_screen->diff)[cblock_x][cblock_y];
        }
    }
    return mod;
}

void gen_pblock_bitmap(CharScreen *char_screen, uint8_t pblock_x,
                       uint8_t pblock_y, PblockBitmap *pblock_bitmap)
{
    uint8_t cblock_x = pblock_x;
    int8_t first_cblock = P2B_MAP[pblock_y][0];
    int8_t last_cblock = P2B_MAP[pblock_y][1];
    int8_t cblock_y = 0;
    uint8_t first_line = 0;
    uint8_t last_line = 0;
    uint8_t cblock_line = 0;
    uint64_t character = 0x00000000;
    CarryCounter pblock_y_counter = new_carry_counter(PAGE_HEIGHT, 0, 0);
    for (cblock_y = first_cblock; cblock_y < last_cblock + 1; ++cblock_y)
    {
        first_line = PB2L_MAP[pblock_y][cblock_y + 1][0];
        last_line = PB2L_MAP[pblock_y][cblock_y + 1][1];
        for (cblock_line = first_line; cblock_line < last_line + 1;
                ++cblock_line)
        {
            if ((cblock_y < 0) || (cblock_y > BLOCK_COUNT_Y - 1))
            {
                character = EMPTY_CHAR;
            }
            else
            {
                character = char_screen->content[cblock_x][cblock_y];
            }
            (*pblock_bitmap)[pblock_y_counter.lsb] = CHAR_MAP[character][cblock_line];
            count(&pblock_y_counter);
        }
    }
}

