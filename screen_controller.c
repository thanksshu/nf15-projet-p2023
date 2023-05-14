#include "includes.h"

ScreenController new_screen_controller()
{
    ScreenController screen_controller;
    return char_screen;
}

void change_raw_screen_cursor(ScreenController *screen_controller, int page,
                              int column)
{
    if (screen_controller->raw_screen_current_page != page)
    {
        select_raw_screen_page(page);
        screen_controller->raw_screen_current_page = page;
    }
    if (screen_controller->raw_screen_current_column != column)
    {
        select_raw_screen_column(column);
        screen_controller->raw_screen_current_column = column;
    }
}

void set_screen_px(ScreenController *screen_controller, int x, int y, bool px)
{
    int page = y / PAGE_COUNT;
    int bit = y % PAGE_COUNT;
    uint8_t bit_mask = 1 << bit;

    int col = y;

    uint8_t *bitmap = screen_controller->content[page][col].next_bitmap;
    *bitmap = (*bitmap & ~bit_mask | px << bit);
}

void draw_screen(ScreenController *screen_controller)
{
    int page = 0;
    int column = 0;
    for (page = 0; page < PAGE_COUNT; ++page)
    {
        for (column = 0; column < SCREEN_WIDTH; ++column)
        {
            uint8_t *next_bitmap =
                    screen_controller->content[page][column].next_bitmap;
            uint8_t *current_bitmap =
                    screen_controller->content[page][column].current_bitmap;

            // Draw only when bitmap changed
            if (*next_bitmap != *current_bitmap)
            {
                change_raw_screen_cursor(screen_controller, page, column);
                draw_raw_screen_page_column(next_bitmap);
                *current_bitmap = *next_bitmap;
                screen_controller->raw_screen_current_column += 1; // This is done automatically by the raw screen
            }
        }
    }
}

void draw_from_page_block_bitmap(RawScreen *raw_screen, uint8_t page_block_x,
                                 uint8_t page_block_y,
                                 PageBlockBitmap *page_block_bitmap)
{
    uint8_t pblock_row = 0;
    uint8_t row_bitmap = 0;
    BlockRowBitmap mask = 0;

    printf("+%d++%d+\n", page_block_x, page_block_y);
    for (pblock_row = 0; pblock_row < PAGE_HEIGHT; ++pblock_row)
    {
        row_bitmap = (*page_block_bitmap)[pblock_row];
        for (mask = 1 << (BLOCK_HALF_WIDTH - 1); mask != 0; mask >>= 1)
        {
            // TODO: Draw from page block bitmap
            if (row_bitmap & mask)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("\n");
    }
}

uint8_t gen_pblock_col_bitmap(PageBlockBitmap *page_block_bitmap,
                              uint8_t bitmap_col)
{
    char bit = 0; // for only 1 bit
    uint8_t pblock_col_bitmap = 0;
    uint8_t bitmap_row = 0;
    for (bitmap_row = 0; bitmap_row < PAGE_HEIGHT + 1; ++bitmap_row)
    {
        bit = ((*page_block_bitmap)[bitmap_row]
                >> (BLOCK_HALF_WIDTH - 1 - bitmap_col)) & 1; // pick the bit at pblock_col
        pblock_col_bitmap |= bit << bitmap_row; // put at bitmap_col
    }
    return pblock_col_bitmap;
}
