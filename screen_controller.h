/*
 * Controller for the "physical screen" (raw screen)
 */

#include "includes.h"

#ifndef RAW_SCREEN_H_
#define RAW_SCREEN_H_

/**
 * Raw screen page column bitmap
 */
typedef struct {
    uint8_t next_bitmap; // bitmap for next draw call, MSB first
    uint8_t current_bitmap; // current bitmap on raw screen, MSB first
} RawScreenPageColumn;

/**
 * Represents the "physical screen"
 */
typedef struct
{
    RawScreenPageColumn content[PAGE_COUNT][SCREEN_WIDTH];

    // Raw screen's current selected page and column
    int raw_screen_current_page;
    int raw_screen_current_column;
} ScreenController;

#endif /* RAW_SCREEN_H_ */
