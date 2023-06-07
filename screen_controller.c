#include "includes.h"

ScreenController new_screen_controller()
{
    ScreenController screen_controller;

    init_physical_screen_communication();
    init_physical_screen_display();

    draw_buffer_screen_all(&screen_controller, CLEAR_SCREEN_COLOR);
    sync_screen(&screen_controller, true); // Force to clear screen

    return screen_controller;
}

void draw_buffer_screen_pixel(ScreenController *screen_controller, int x, int y,
                              Color color)
{
    unsigned int page = x / PAGE_COUNT;
    unsigned int bit = x % PAGE_COUNT;
    unsigned int col = y;

    if (page >= PAGE_COUNT || bit >= PAGE_HEIGHT || col >= SCREEN_WIDTH)
    {
        return; // Boundary check
    }

    uint8_t mask = 1 << bit;

    uint8_t *bitmap = &(screen_controller->buffer_content[page][col]);
    // Set the needed pixel only, colour need to be inverted cause the screen use 1 as display flag
    *bitmap = (*bitmap & ~mask) | (!color << bit);
}

void draw_buffer_screen_bitmap(ScreenController *screen_controller, int x,
                               int y, Bitmap *bitmap)
{
    Alpha alpha = false;
    Color color = false;
    size_t pixel_index = 0;
    size_t pixel_x = 0;
    size_t pixel_y = 0;
    for (pixel_x = 0; pixel_x < bitmap->height; ++pixel_x)
    {
        for (pixel_y = 0; pixel_y < bitmap->width; ++pixel_y)
        {
            pixel_index = pixel_y + bitmap->width * pixel_x;
            alpha = *(bitmap->alpha + pixel_index);
            if (alpha) // Only draw when solid
            {
                color = *(bitmap->color + pixel_index);
                draw_buffer_screen_pixel(screen_controller, x + pixel_x,
                                         y + pixel_y, color);
            }
        }
    }
}

void draw_buffer_screen_bitmaps_on_row(ScreenController *screen_controller,
                                       int x, int y, Bitmap **bitmaps,
                                       int length)
{
    size_t index = 0;
    size_t current_x = x;
    size_t current_y = y;
    for (index = 0; index < length; ++index)
    {
        draw_buffer_screen_bitmap(screen_controller, current_x, current_y,
                                  *(bitmaps + index));
        current_y += (*(bitmaps + index))->width;
    }
}

void draw_buffer_screen_all(ScreenController *screen_controller, Color color)
{
    size_t x = 0;
    size_t y = 0;
    for (x = 0; x < SCREEN_HEIGHT; ++x)
    {
        for (y = 0; y < SCREEN_WIDTH; ++y)
        {
            draw_buffer_screen_pixel(screen_controller, x, y, color);
        }
    }
}

void sync_screen(ScreenController *screen_controller, bool force_sync)
{
    // Sync screen content
    size_t page = 0;
    size_t column = 0;
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

