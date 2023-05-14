/*
 * unicode_bitmap_map.h
 * Getter for UNICODE_BITMAP_MAP in config.h
 */
#include "includes.h"

#ifndef UNICODE_BITMAP_MAP_H_
#define UNICODE_BITMAP_MAP_H_

uint16_t get_row_bitmap(uint16_t *unicode_bitmap_map[MAX_INDEX][BLOCK_HEIGHT], uint64_t index, uint8_t block_row);

#endif /* UNICODE_BITMAP_MAP_H_ */
