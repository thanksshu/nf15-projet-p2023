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
#define CLEAR_SCREEN_COLOR 1
#define GLYPH_WIDTH 12 // Width of a glyph. Max value is MAX_GLYPH_WIDTH
#define GLYPH_HEIGHT 12

/* Constants, DON'T TOUCH */
#define PAGE_COUNT (SCREEN_HEIGHT / PAGE_HEIGHT)
#define GLYPH_HALFWIDTH (GLYPH_WIDTH / 2)
#define MAX_GLYPH_WIDTH 16 // Use uint16_t to store bitmap

#endif /* CONFIG_H_ */
