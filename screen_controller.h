/*
 * Controller for the "physical screen" (raw screen)
 */

#include "includes.h"

#ifndef SCREEN_CONTROLLER_H_
#define SCREEN_CONTROLLER_H_

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
    bool display_on;
} ScreenController;

/**
 * Change raw screen cursor
 * Will not change if already there, can be forced
 */
void change_raw_screen_cursor(ScreenController *screen_controller, int page,
                              int column, bool force_change);

/**
 * Draw on raw screen
 * Will not draw pixels that are not changed, but can be forced to draw all
 */
void draw_screen(ScreenController *screen_controller, bool draw_all);

/**
 * Turn on raw screen
 */
void turn_screen_on(ScreenController *screen_controller);

/**
 * Turn on off screen
 */
void turn_screen_off(ScreenController *screen_controller);

/**
 * Factory method for screen controller
 * Raw screen will be cleared and its cursor will be set (0, 0)
 */
ScreenController new_screen_controller();

/**
 * Set one pixel on screen controller
 * Changes will be drawn on next draw call
 */
void set_screen_px(ScreenController *screen_controller, int x, int y, bool px);

/**
 * Clear all pixel on screen controller
 * Changes will be drawn on next draw call
 */
void clear_screen(ScreenController *screen_controller);

#endif /* SCREEN_CONTROLLER_H_ */
