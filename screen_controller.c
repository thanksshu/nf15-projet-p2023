#include "includes.h"

ScreenController new_screen_controller()
{
    ScreenController screen_controller;

    init_raw_screen_communication();
    init_raw_screen_display();

    set_buffer_screen_display_on(&screen_controller, false); // Display is off when start up
    set_screen_cursor(&screen_controller, 0, 0, true); // Force initialise cursor to (0, 0)
    set_all_buffer_screen_pixel(&screen_controller, 1); // Use white as cleared screen colour
    sync_screen(&screen_controller, true); // Force to clear screen

    return screen_controller;
}

void set_screen_cursor(ScreenController *screen_controller, int page,
                       int column, bool force_set)
{
    if (force_set || screen_controller->physical_cursor_page != page)
    {
        set_raw_screen_page(page);
        screen_controller->physical_cursor_page = page;
    }

    if (force_set || (screen_controller->physical_cursor_column != column))
    {
        set_raw_screen_column(column);
        screen_controller->physical_cursor_column = column;
    }
}

void set_buffer_screen_display_on(ScreenController *screen_controller, bool on)
{
    screen_controller->buffer_display_on = on;
}

void set_buffer_screen_pixel(ScreenController *screen_controller, int x, int y,
                             Color color)
{
    int page = x / PAGE_COUNT;
    int bit = x % PAGE_COUNT;
    int col = y;
    uint8_t mask = 1 << bit;

    uint8_t *bitmap = &(screen_controller->buffer_content[page][col]);
    *bitmap = (*bitmap & ~mask) | (!color << bit); // Set the needed pixel only, colour need to be inverted
}

void set_buffer_screen_bitmap(ScreenController *screen_controller, int x, int y,
                              Bitmap *bitmap)
{
    Alpha alpha = false;
    Color color = false;
    int pixel_index = 0;
    int pixel_x = 0;
    int pixel_y = 0;
    for (pixel_x = 0; pixel_x < bitmap->height; ++pixel_x)
    {
        for (pixel_y = 0; pixel_y < bitmap->width; ++pixel_y)
        {
            pixel_index = pixel_y + bitmap->width * pixel_x;
            alpha = *(bitmap->alpha + pixel_index);
            if (alpha) // Only draw when solid
            {
                color = *(bitmap->color + pixel_index);
                set_buffer_screen_pixel(screen_controller, x + pixel_x,
                                        y + pixel_y, color);
            }
        }
    }
}

void set_all_buffer_screen_pixel(ScreenController *screen_controller,
                                 Color color)
{
    int x = 0;
    int y = 0;
    for (x = 0; x < SCREEN_HEIGHT; ++x)
    {
        for (y = 0; y < SCREEN_WIDTH; ++y)
        {
            set_buffer_screen_pixel(screen_controller, x, y, color);
        }
    }
}

void sync_screen(ScreenController *screen_controller, bool force_sync)
{
    // Sync screen content
    int page = 0;
    int column = 0;
    for (page = 0; page < PAGE_COUNT; ++page)
    {
        for (column = 0; column < SCREEN_WIDTH; ++column)
        {
            uint8_t *next_bitmap =
                    &(screen_controller->buffer_content[page][column]);
            uint8_t *current_bitmap =
                    &(screen_controller->physical_content[page][column]);

            // Draw only when bitmap changed or when forced sync
            if (force_sync || (*next_bitmap != *current_bitmap))
            {
                set_screen_cursor(screen_controller, page, column, false);
                draw_raw_screen_page_column(*next_bitmap);

                // Update screen controller
                screen_controller->physical_cursor_column += 1; // After draw, the column will be increased by the physical screen
                *current_bitmap = *next_bitmap;
            }
        }
    }

    // Sync display on/off
    if (screen_controller->buffer_display_on
            != screen_controller->physical_display_on)
    {
        switch (screen_controller->buffer_display_on)
        {
        case true:
            turn_raw_screen_on();
            break;
        case false:
            turn_raw_screen_off();
            break;
        }
        screen_controller->physical_display_on =
                screen_controller->buffer_display_on;
    }
}

