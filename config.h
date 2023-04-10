/*
 * config.h
 *
 * Configurations and predefined elements here
 */

#include "includes.h"

#ifndef CONFIG_H_
#define CONFIG_H_

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define PAGE_HEIGHT 8
#define BLOCK_WIDTH 6
#define BLOCK_HEIGHT 12
#define EMPTY_CHAR 0
#define CHAR_MAP_SIZE 2

#define PAGE_COUNT (SCREEN_HEIGHT / PAGE_HEIGHT)
#define BLOCK_COUNT_X (SCREEN_WIDTH / BLOCK_WIDTH)
#define BLOCK_COUNT_Y (SCREEN_HEIGHT / BLOCK_HEIGHT)

typedef int8_t PageToBlockMap[PAGE_COUNT][2];
typedef uint8_t PageBlockToLineMap[PAGE_COUNT][BLOCK_COUNT_Y + 2][2];
extern const PageToBlockMap P2B_MAP;
extern const PageBlockToLineMap PB2L_MAP;

typedef int8_t PblockBitmap[PAGE_HEIGHT]; // content type must be no shorter than BLOCK_WIDTH
typedef int8_t UnicodeToBitmap[CHAR_MAP_SIZE][BLOCK_HEIGHT]; // same as above
extern const UnicodeToBitmap CHAR_MAP;

#endif /* CONFIG_H_ */
