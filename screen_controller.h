/*
 * Controller for the physical screen
 */

#include "includes.h"

#ifndef SCREEN_CONTROLLER_H_
#define SCREEN_CONTROLLER_H_

typedef bool Color; // 1 for white, 0 for black
typedef bool Alpha; // 1 for coloured, 0 for transparent

/**
 * A bitmap map for draw image on screen
 */
typedef struct
{
    int height;
    int width;
    Color *color;
    Alpha *alpha;
} Bitmap;

/**
 * Represents the "physical screen"
 * We don't care about the position of the cursor
 */
typedef struct
{
    uint8_t physical_content[PAGE_COUNT][SCREEN_WIDTH];
    uint8_t buffer_content[PAGE_COUNT][SCREEN_WIDTH];
} ScreenController;

/**
 * Factory method for screen controller
 * Raw screen will be cleared
 */
ScreenController new_screen_controller();

/**
 * Draw screen buffer to raw screen
 * Only sync when properties changed, can be forced to sync
 */
void sync_screen(ScreenController *screen_controller, bool force_sync);

/**
 * Set one pixel on screen buffer
 */
void set_buffer_screen_pixel(ScreenController *screen_controller, int x, int y, Color color);

/**
 * Set pixels according to a bitmap
 */
void draw_buffer_screen_bitmap(ScreenController *screen_controller, int x, int y,
                              Bitmap *bitmap);

/**
 * Set bitmaps along a row, with a spacing of 1
 */
void draw_buffer_screen_bitmaps_on_row(ScreenController *screen_controller,
                                       int x, int y, Bitmap **bitmaps, int length);

/**
 * Set all pixel on screen buffer
 * Changes will drawn to raw screen on next draw_screen call
 * Usually used for clear raw screen
 */
void draw_buffer_screen_all(ScreenController *screen_controller, Color color);

#endif /* SCREEN_CONTROLLER_H_ */
