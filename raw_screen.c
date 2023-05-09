#include "includes.h"

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

uint8_t _gen_pblock_col_bitmap(PageBlockBitmap *page_block_bitmap,
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
