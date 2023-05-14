#include "includes.h"

uint16_t get_row_bitmap(uint16_t *unicode_bitmap_map[MAX_INDEX][BLOCK_HEIGHT],
                        uint64_t index, uint8_t block_row)
{
    return *unicode_bitmap_map[index][block_row];
}
