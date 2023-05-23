#include "includes.h"

ScreenController new_screen_controller()
{
    ScreenController screen_controller;

    init_physical_screen_communication();
    init_physical_screen_display();

    set_buffer_screen_all(&screen_controller, 1); // Use white as cleared screen colour
    sync_screen(&screen_controller, true); // Force to clear screen

    return screen_controller;
}

void turn_screen_on_off(ScreenController *screen_controller, bool on)
{
    if (on)
    {
        turn_physical_screen_on();
    }
    else
    {
        turn_physical_screen_off();
    }
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

void set_buffer_screen_all(ScreenController *screen_controller, Color color)
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
                set_physical_screen_page(page);
                set_physical_screen_column(column);
                draw_physical_screen_page_column(*next_bitmap);

                // Update screen controller
                *current_bitmap = *next_bitmap;
            }
        }
    }
}

