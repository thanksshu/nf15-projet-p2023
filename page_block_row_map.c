#include "includes.h"

CarryCounter new_carry_counter(int16_t carry_when, int16_t initial_msb,
                               int16_t initial_lsb)
{
    CarryCounter carry_counter;
    carry_counter.carry_when = carry_when;
    carry_counter.msb = initial_msb;
    carry_counter.lsb = initial_lsb;
    return carry_counter;
}

void count_carry_counter(CarryCounter *carry_counter)
{
    // Self increase, then carry if needed
    if (carry_counter->lsb < (carry_counter->carry_when - 1))
    {
        carry_counter->lsb += 1;
    }
    else
    {
        carry_counter->msb += 1;
        carry_counter->lsb = 0;
    }
}

PageBlockRowMap new_page_block_row_map()
{
    PageBlockRowMap page_block_row_map;

    // Initialise page_to_blocks array
    uint8_t page = 0;
    int16_t block = 0;
    for (page = 0; page < PAGE_COUNT; ++page)
    {
        page_block_row_map.page_to_block_range[page][0] = BLOCK_HEIGHT - 1;

        page_block_row_map.page_to_block_range[page][1] = 0;
    }

    // Initialise page_block_to_lines array
    for (page = 0; page < PAGE_COUNT; ++page)
    {
        for (block = 0; block < BLOCK_COUNT_Y + 2; block++)
        {
            page_block_row_map.page_block_to_line_range[page][block][0] =
            BLOCK_HEIGHT - 1;
            page_block_row_map.page_block_to_line_range[page][block][1] = 0;
        }
    }

    _set_page_block_row_map_content(&page_block_row_map);

    return page_block_row_map;
}

void _adjust_page_to_blocks_range(PageBlockRowMap *page_block_row_map,
                                  uint8_t page, int16_t block)
{
    if (block < page_block_row_map->page_to_block_range[page][0])
    {
        page_block_row_map->page_to_block_range[page][0] = block;
    }
    if (block > page_block_row_map->page_to_block_range[page][1])
    {
        page_block_row_map->page_to_block_range[page][1] = block;
    }
}

void _adjust_page_block_to_row_range(PageBlockRowMap *page_block_row_map,
                                     uint8_t page, int16_t block, uint8_t line)
{
    uint8_t _block = block + 1; // "+ 1" for the block that don't exist
    if (line < page_block_row_map->page_block_to_line_range[page][_block][0])
    {
        page_block_row_map->page_block_to_line_range[page][_block][0] = line;
    }
    if (line > page_block_row_map->page_block_to_line_range[page][_block][1])
    {
        page_block_row_map->page_block_to_line_range[page][_block][1] = line;
    }
}

void _set_page_block_row_map_content(PageBlockRowMap *page_block_row_map)
{
    // The starting page's first block, either -1 or 0, we perform a floor division
    int16_t first_block = (int16_t) floor(
            ((double) (0 * PAGE_HEIGHT - MARGIN_TOP))
                    / ((double) BLOCK_HEIGHT));
    // And the first line
    int16_t first_block_line = (0 * PAGE_HEIGHT - MARGIN_TOP)
            - first_block * BLOCK_HEIGHT;

    CarryCounter block_counter = new_carry_counter(BLOCK_HEIGHT, first_block,
                                                   first_block_line);

    uint8_t page = 0;
    uint8_t line = 0;
    for (page = 0; page < PAGE_COUNT; ++page)
    {
        for (line = 0; line < PAGE_HEIGHT; ++line)
        {
            _adjust_page_to_blocks_range(page_block_row_map, page,
                                         block_counter.msb);
            _adjust_page_block_to_row_range(page_block_row_map, page,
                                            block_counter.msb,
                                            block_counter.lsb);
            count_carry_counter(&block_counter);
        }
    }
}

int16_t get_page_first_block(PageBlockRowMap *page_block_row_map, uint8_t page)
{
    return page_block_row_map->page_to_block_range[page][0];
}

int16_t get_page_last_block(PageBlockRowMap *page_block_row_map, uint8_t page)
{
    return page_block_row_map->page_to_block_range[page][1];
}

uint8_t get_page_block_first_row(PageBlockRowMap *page_block_row_map,
                                 uint8_t page, int16_t block)
{
    int block_index = block + 1; // "+ 1" for those block that don't exist
    return page_block_row_map->page_block_to_line_range[page][block_index][0];
}

uint8_t get_page_block_last_row(PageBlockRowMap *page_block_row_map,
                                uint8_t page, int16_t block)
{
    int block_index = block + 1; // "+ 1" for the block that don't exist
    return page_block_row_map->page_block_to_line_range[page][block_index][1];
}

