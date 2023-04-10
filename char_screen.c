#include "includes.h"

CharScreen new_char_screen()
{
    CharScreen char_screen;
    clear_content(&char_screen);
    _clear_diff(&char_screen);
    return char_screen;
}

void set_block(CharScreen *char_screen, uint8_t x, uint8_t y,
               uint64_t character)
{
    if ((char_screen->_content)[x][y] == character)
    {
        return;
    }
    (char_screen->_is_mod)[x][y] = true;
    (char_screen->_content)[x][y] = character;
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

void _clear_diff(CharScreen *char_screen)
{
    uint8_t x = 0;
    uint8_t y = 0;
    for (x = 0; x < BLOCK_COUNT_X; ++x)
    {
        for (y = 0; y < BLOCK_COUNT_Y; ++y)
        {
            (char_screen->_is_mod)[x][y] = false;
        }
    }
}

void draw(CharScreen *char_screen, RawScreen *raw_screen)
{
    uint8_t pblock_x = 0;
    uint8_t pblock_y = 0;
    PblockBitmap pblock_bitmap = { 0 };
    // Iterate over all page blocks and check if modification needed.
    for (pblock_y = 0; pblock_y < PAGE_COUNT; ++pblock_y)
    {
        for (pblock_x = 0; pblock_x < BLOCK_COUNT_X; ++pblock_x)
        {
            if (_need_mod(char_screen, pblock_x, pblock_y))
            {
                _gen_pblock_bitmap(char_screen, pblock_x, pblock_y,
                                   &pblock_bitmap);
                draw_from_pblock_bitmap(raw_screen, pblock_x, pblock_y,
                                        &pblock_bitmap);
            }
        }
    }
    _clear_diff(char_screen);
}

bool _need_mod(CharScreen *char_screen, uint8_t pblock_x, uint8_t pblock_y)
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
            mod = mod || (char_screen->_is_mod)[cblock_x][cblock_y];
        }
    }
    return mod;
}

void _gen_pblock_bitmap(CharScreen *char_screen, uint8_t pblock_x,
                        uint8_t pblock_y, PblockBitmap *pblock_bitmap)
{
    uint8_t cblock_x = pblock_x;

    int8_t first_cblock = P2B_MAP[pblock_y][0];
    int8_t last_cblock = P2B_MAP[pblock_y][1];

    uint8_t first_cblock_line = 0;
    uint8_t last_cblock_line = 0;

    uint64_t character = 0x0000;

    int8_t cblock_y = 0;
    uint8_t pblock_line = 0;
    uint8_t cblock_line = 0;
    for (cblock_y = first_cblock; cblock_y < last_cblock + 1; ++cblock_y)
    {
        first_cblock_line = PB2L_MAP[pblock_y][cblock_y + 1][0];
        last_cblock_line = PB2L_MAP[pblock_y][cblock_y + 1][1];
        for (cblock_line = first_cblock_line;
                cblock_line < last_cblock_line + 1; ++cblock_line)
        {
            if ((cblock_y < 0) || (cblock_y > BLOCK_COUNT_Y - 1))
            {
                character = EMPTY_CHAR;
            }
            else
            {
                character = char_screen->_content[cblock_x][cblock_y];
            }
            (*pblock_bitmap)[pblock_line] = CHAR_MAP[character][cblock_line];
            pblock_line += 1;
        }
    }
}

void test_char_screen()
{
    CharScreen char_screen = new_char_screen();
    RawScreen raw_screen;
    set_block(&char_screen, 0, 0, 1);
    set_block(&char_screen, 0, 1, 1);
    set_block(&char_screen, 0, 2, 1);
    set_block(&char_screen, 0, 3, 1);
    draw(&char_screen, &raw_screen);
    printf("------\n");
    clear_content(&char_screen);
    draw(&char_screen, &raw_screen);
    printf("------\n");
    set_block(&char_screen, 2, 0, 1);
    set_block(&char_screen, 2, 0, 1);
    set_block(&char_screen, 2, 1, 1);
    set_block(&char_screen, 2, 3, 1);
    set_block(&char_screen, 2, 4, 1);
    draw(&char_screen, &raw_screen);
    printf("------\n");
}

