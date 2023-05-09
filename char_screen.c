#include "includes.h"

CharScreen new_char_screen()
{
    CharScreen char_screen;
    clear_char_screen(&char_screen);
    _clear_char_screen_diff(&char_screen);
    return char_screen;
}

void set_char_block(CharScreen *char_screen, uint8_t x, uint8_t y,
                    Unicode character, Side side)
{
    if ((char_screen->_char_info)[x][y].character == character
            && (char_screen->_char_info)[x][y].side == side)
    {
        (char_screen->_char_info)[x][y].need_mod = false;
        (char_screen->_char_info)[x][y].next_side =
                (char_screen->_char_info)[x][y].side;
        (char_screen->_char_info)[x][y].next_char =
                (char_screen->_char_info)[x][y].character;
    }
    else
    {
        (char_screen->_char_info)[x][y].need_mod = true;
        (char_screen->_char_info)[x][y].next_side = side;
        (char_screen->_char_info)[x][y].next_char = character;
    }
}

void clear_char_screen(CharScreen *char_screen)
{
    uint8_t char_block_x = 0;
    uint8_t char_block_y = 0;
    for (char_block_x = 0; char_block_x < BLOCK_COUNT_X; ++char_block_x)
    {
        for (char_block_y = 0; char_block_y < BLOCK_COUNT_Y; ++char_block_y)
        {
            set_char_block(char_screen, char_block_x, char_block_y, EMPTY_CHAR,
            EMPTY_CHAR_SIDE);
        }
    }
}

void _clear_char_screen_diff(CharScreen *char_screen)
{
    uint8_t x = 0;
    uint8_t y = 0;
    for (x = 0; x < BLOCK_COUNT_X; ++x)
    {
        for (y = 0; y < BLOCK_COUNT_Y; ++y)
        {
            (char_screen->_char_info)[x][y].need_mod = false;
        }
    }
}

void draw_char_screen(CharScreen *char_screen,
                      PageBlockRowMap *page_block_row_map,
                      UnicodeBitmapMapPtr *unicode_bitmap, RawScreen *raw_screen)
{
    uint8_t page_block_x = 0;
    uint8_t page_block_y = 0;
    PageBlockBitmap page_block_bitmap = { 0 };
    // Iterate over all page blocks and check if modification needed.
    for (page_block_y = 0; page_block_y < PAGE_COUNT; ++page_block_y)
    {
        for (page_block_x = 0; page_block_x < BLOCK_COUNT_X; ++page_block_x)
        {
            if (_is_page_block_need_mod(char_screen, page_block_row_map,
                                        page_block_x, page_block_y))
            {
                _gen_page_block_bitmap(char_screen, page_block_row_map,
                                       unicode_bitmap, page_block_x,
                                       page_block_y, &page_block_bitmap);
                draw_from_page_block_bitmap(raw_screen, page_block_x,
                                            page_block_y, &page_block_bitmap);
            }
        }
    }
    _clear_char_screen_diff(char_screen);
}

bool _is_page_block_need_mod(CharScreen *char_screen,
                             PageBlockRowMap *page_block_row_map,
                             uint8_t page_block_x, uint8_t page_block_y)
{
    uint8_t char_block_x = page_block_x;

    int16_t first_char_block = get_page_first_block(page_block_row_map,
                                                    page_block_y);
    int16_t last_char_block = get_page_last_block(page_block_row_map,
                                                  page_block_y);

    int16_t char_block_y = 0;
    for (char_block_y = first_char_block; char_block_y < last_char_block + 1;
            ++char_block_y)
    {
        if ((char_block_y < 0) || (char_block_y > BLOCK_COUNT_Y - 1))
        {
            continue; // Consider as not modified
        }
        if ((char_screen->_char_info)[char_block_x][char_block_y].need_mod
                == true)
        {
            return true;
        }
    }
    return false;
}

void _gen_page_block_bitmap(CharScreen *char_screen,
                            PageBlockRowMap *page_block_row_map,
                            UnicodeBitmapMapPtr *unicode_bitmap,
                            uint8_t page_block_x, uint8_t page_block_y,
                            PageBlockBitmap *page_block_bitmap)
{
    uint8_t char_block_x = page_block_x;

    int16_t first_char_block = get_page_first_block(page_block_row_map,
                                                    page_block_y);
    int16_t last_char_block = get_page_last_block(page_block_row_map,
                                                  page_block_y);

    uint8_t first_char_block_row = 0;
    uint8_t last_char_block_row = 0;

    int16_t char_block_y = 0;
    uint8_t page_block_row = 0;
    uint8_t char_block_row = 0;
    for (char_block_y = first_char_block; char_block_y < last_char_block + 1;
            ++char_block_y)
    {
        first_char_block_row = get_page_block_first_row(page_block_row_map,
                                                        page_block_y,
                                                        char_block_y);
        last_char_block_row = get_page_block_last_row(page_block_row_map,
                                                      page_block_y,
                                                      char_block_y);
        for (char_block_row = first_char_block_row;
                char_block_row < last_char_block_row + 1; ++char_block_row)
        {
            (*page_block_bitmap)[page_block_row] = _gen_page_block_row_bitmap(
                    char_screen, unicode_bitmap, char_block_x, char_block_y,
                    char_block_row);
            page_block_row += 1;
        }
    }
}

BlockRowBitmap _gen_page_block_row_bitmap(CharScreen *char_screen,
                                          UnicodeBitmapMapPtr *unicode_bitmap,
                                          uint8_t char_block_x,
                                          int16_t char_block_y,
                                          uint8_t char_block_row)
{
    Side side = Left;
    Unicode character = 0x0000;
    if ((char_block_y < 0) || (char_block_y > BLOCK_COUNT_Y - 1))
    {
        character = EMPTY_CHAR;
        side = EMPTY_CHAR_SIDE;
    }
    else
    {
        character =
                (char_screen->_char_info)[char_block_x][char_block_y].next_char;
        side = (char_screen->_char_info)[char_block_x][char_block_y].next_side;

        (char_screen->_char_info)[char_block_x][char_block_y].character =
                (char_screen->_char_info)[char_block_x][char_block_y].next_char;
        (char_screen->_char_info)[char_block_x][char_block_y].side =
                (char_screen->_char_info)[char_block_x][char_block_y].next_side;
    }

//    BlockRowBitmap fullwidth_row_bitmap = get_unicode_row_bitmap(
//            unicode_bitmap, character, char_block_row);
    BlockRowBitmap fullwidth_row_bitmap = 0;
//            UNICODE_BITMAP_MAP[character][char_block_row];

    BlockRowBitmap halfwidth_row_bitmap = 0;
    if (side == Right)
    {
        halfwidth_row_bitmap = fullwidth_row_bitmap & BLOCK_BITMAP_HALF_MASK;
    }
    else
    {
        halfwidth_row_bitmap = fullwidth_row_bitmap >> BLOCK_HALF_WIDTH;
    }
    return halfwidth_row_bitmap;

}

//const uint16_t UNICODE_BITMAP_MAP[2][BLOCK_HEIGHT] = { { 0b000000000000,
//                                                         0b000000000000,
//                                                         0b011100000000,
//                                                         0b100010000000,
//                                                         0b100110000000,
//                                                         0b101010000000,
//                                                         0b101010000000,
//                                                         0b110010000000,
//                                                         0b100010000000,
//                                                         0b011100000000,
//                                                         0b000000000000,
//                                                         0b000000000000 },
//                                                       { 0b000000000000,
//                                                         0b000000000000,
//                                                         0b010000000000,
//                                                         0b110000000000,
//                                                         0b010000000000,
//                                                         0b010000000000,
//                                                         0b010000000000,
//                                                         0b010000000000,
//                                                         0b010000000000,
//                                                         0b111000000000,
//                                                         0b000000000000,
//                                                         0b000000000000 } };

