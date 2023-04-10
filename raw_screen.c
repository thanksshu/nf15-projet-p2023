#include "includes.h"

void draw_from_pblock_bitmap(RawScreen *raw_screen, uint8_t pblock_x,
                             uint8_t pblock_y, PblockBitmap *pblock_bitmap)
{
    uint8_t pblock_line = 0;
    uint8_t line_bitmap = 0;
    uint8_t mask = 0;

    printf("------\n");
    for (pblock_line = 0; pblock_line < PAGE_HEIGHT; ++pblock_line)
    {
        line_bitmap = (*pblock_bitmap)[pblock_line];
        for (mask = 1 << BLOCK_WIDTH; mask != 0; mask >>= 1)
        {
            // TODO: Draw from pblock bitmap
            if (line_bitmap & mask)
                printf(" ");
            else
                printf("#");
        }
        printf("\n");
    }
}
