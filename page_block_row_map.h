/*
 * page_block_row_map.h
 *
 *
 */
#include "includes.h"

#ifndef PAGE_BLOCK_ROW_MAP_H_
#define PAGE_BLOCK_ROW_MAP_H_

/**
 * A counter that carries
 */
typedef struct
{
    int16_t carry_when;
    int16_t msb;
    int16_t lsb;
} CarryCounter;

/**
 * CarryCounter factory function
 */
CarryCounter new_carry_counter(int16_t carry_when, int16_t initial_msb,
                               int16_t initial_lsb);
/**
 * Count one more for CarryCounter
 */
void count_carry_counter(CarryCounter *carry_counter);


/**
 * The page to block to row mapping
 */
typedef struct
{
    // Needed block range for given page
    int16_t page_to_block_range[PAGE_COUNT][2];

    // Needed line range for given page and its block
    // Because of the margin, we need two more "virtual block" for the -1 block,
    // and the BLOCK_COUNT_Y block
    // Fortunately, as we use the modulo operation, "+2" is just enough
    uint8_t page_block_to_line_range[PAGE_COUNT][BLOCK_COUNT_Y + 2][2];
} PageBlockRowMap;

/**
 * PageBlockRowMap factory function
 */
PageBlockRowMap new_page_block_row_map();

/**
 * Initialisers
 */
void _adjust_page_to_blocks_range(PageBlockRowMap *page_block_line_map,
                                  uint8_t page, int16_t block);
void _adjust_page_block_to_row_range(PageBlockRowMap *page_block_line_map,
                                     uint8_t page, int16_t block, uint8_t line);
void _set_page_block_row_map_content(PageBlockRowMap *page_block_row_map);

/**
 * Getters
 */
int16_t get_page_first_block(PageBlockRowMap *page_block_row_map, uint8_t page);
int16_t get_page_last_block(PageBlockRowMap *page_block_row_map, uint8_t page);
uint8_t get_page_block_first_row(PageBlockRowMap *page_block_row_map,
                                 uint8_t page, int16_t block);
uint8_t get_page_block_last_row(PageBlockRowMap *page_block_row_map,
                               uint8_t page, int16_t block);
#endif /* PAGE_BLOCK_ROW_MAP_H_ */
