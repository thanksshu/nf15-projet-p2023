/*
 * page_block_row_map.h
 *
 * Getters for PAGE_TO_BLOCK_RANGE and PAGE_BLOCK_TO_ROW_RANGE in config.h
 */
#include "includes.h"

#ifndef PAGE_BLOCK_ROW_MAP_H_
#define PAGE_BLOCK_ROW_MAP_H_

/**
 * Getters
 */
int16_t get_page_first_block(int16_t *page_to_block_range[PAGE_COUNT][2], uint8_t page);
int16_t get_page_last_block(int16_t *page_to_block_range[PAGE_COUNT][2], uint8_t page);
uint8_t get_page_block_first_row(uint8_t *page_block_to_row_range[PAGE_COUNT][BLOCK_COUNT_Y + 2][2],
                                 uint8_t page, int16_t block);
uint8_t get_page_block_last_row(uint8_t *page_block_to_row_range[PAGE_COUNT][BLOCK_COUNT_Y + 2][2],
                               uint8_t page, int16_t block);
#endif /* PAGE_BLOCK_ROW_MAP_H_ */
