/*
 * config.h
 *
 * Configurations and predefined elements here
 */

#include "includes.h"

#ifndef CONFIG_H_
#define CONFIG_H_

/* Configuration */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define PAGE_HEIGHT 8 // MUST be exactly divisible by the SCREEN_HEIGHT

#define MAX_INDEX 2
#define EMPTY_CHAR 0
#define EMPTY_CHAR_SIDE Left

#define BLOCK_WIDTH 12 // Full width of a glyph provided by the BDF font; Max 16
#define BLOCK_HEIGHT 12 // Value provided by the BDF font; Max 16

/* End configuration */
#define PAGE_COUNT (SCREEN_HEIGHT / PAGE_HEIGHT)
#define BLOCK_HALF_WIDTH (BLOCK_WIDTH / 2)
#define BLOCK_BITMAP_HALF_MASK (1 << BLOCK_HALF_WIDTH - 1)
#define BLOCK_COUNT_X (SCREEN_WIDTH / BLOCK_HALF_WIDTH)
#define BLOCK_COUNT_Y (SCREEN_HEIGHT / BLOCK_HEIGHT)
#define MARGIN_LEFT (SCREEN_WIDTH % BLOCK_HALF_WIDTH / 2)
#define MARGIN_TOP (SCREEN_HEIGHT % BLOCK_HEIGHT / 2)

extern const int16_t PAGE_TO_BLOCK_RANGE[PAGE_COUNT][2];
extern const uint8_t PAGE_BLOCK_TO_ROW_RANGE[PAGE_COUNT][BLOCK_COUNT_Y + 2][2];
extern const uint16_t BITMAP_MAP[MAX_INDEX][BLOCK_HEIGHT];

#endif /* CONFIG_H_ */
