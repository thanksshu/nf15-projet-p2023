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
 * Update raw screen to screen buffer
 * Only sync when properties changed, can be forced to sync
 */
void sync_screen(ScreenController *screen_controller, bool force_sync);

/**
 * Draw one pixel on screen buffer
 */
void draw_buffer_screen_pixel(ScreenController *screen_controller, int x, int y, Color color);

/**
 * Draw pixels according to a bitmap on screen buffer
 */
void draw_buffer_screen_bitmap(ScreenController *screen_controller, int x, int y,
                              Bitmap *bitmap);

/**
 * Draw bitmaps along a row on screen buffer
 */
void draw_buffer_screen_bitmaps_on_row(ScreenController *screen_controller,
                                       int x, int y, Bitmap **bitmaps, int length);

/**
 * Draw all pixel on screen buffer
 */
void draw_buffer_screen_all(ScreenController *screen_controller, Color color);

#endif /* SCREEN_CONTROLLER_H_ */
