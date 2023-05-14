#include "includes.h"

int16_t get_page_first_block(int16_t *page_to_block_range[PAGE_COUNT][2],
                             uint8_t page)
{
    return *page_to_block_range[page][0];
}
int16_t get_page_last_block(int16_t *page_to_block_range[PAGE_COUNT][2],
                            uint8_t page)
{
    return page_to_block_range[page][1];
}
uint8_t get_page_block_first_row(
        uint8_t *page_block_to_row_range[PAGE_COUNT][BLOCK_COUNT_Y + 2][2],
        uint8_t page, int16_t block)
{
    return page_block_to_line_range[page][block + 1][0]; // +1 for block that don't exist
}
uint8_t get_page_block_last_row(
        uint8_t *page_block_to_row_range[PAGE_COUNT][BLOCK_COUNT_Y + 2][2],
        uint8_t page, int16_t block)
{
    return page_block_to_row_range[page][block + 1][1]; // +1 for block that don't exist
}
