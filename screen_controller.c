#include "includes.h"

void change_raw_screen_cursor(ScreenController *screen_controller, int page,
                              int column, bool force_change)
{
    if (force_change || (screen_controller->raw_screen_current_page != page))
    {
        select_raw_screen_page(page);
        screen_controller->raw_screen_current_page = page;
    }

    if (force_change
            || (screen_controller->raw_screen_current_column != column))
    {
        select_raw_screen_column(column);
        screen_controller->raw_screen_current_column = column;
    }
}

ScreenController new_screen_controller()
{
    ScreenController screen_controller;

    init_raw_screen_communication();
    init_raw_screen_display();

    screen_controller.display_on = false;

    // Initialise cursor position
    change_raw_screen_cursor(&screen_controller, 0, 0, true);

    // Clear screen
    clear_screen(&screen_controller);
    draw_screen(&screen_controller, true);

    // Reset cursor to origin
    change_raw_screen_cursor(&screen_controller, 0, 0, false);

    return screen_controller;
}

void turn_screen_on(ScreenController *screen_controller)
{
    screen_controller->display_on = true;
    turn_raw_screen_on();
}

void turn_screen_off(ScreenController *screen_controller)
{
    screen_controller->display_on = false;
    turn_raw_screen_off();
}

void set_screen_px(ScreenController *screen_controller, int x, int y, bool px)
{
    int page = x / PAGE_COUNT;
    int bit = x % PAGE_COUNT;
    uint8_t bit_mask = 1 << bit;

    int col = y;

    uint8_t *bitmap = &(screen_controller->content[page][col].next_bitmap);
    screen_controller->content[page][col].next_bitmap = (*bitmap & ~bit_mask
            | px << bit);
}

void clear_screen(ScreenController *screen_controller)
{
    int x = 0;
    int y = 0;
    for (x = 0; x < SCREEN_HEIGHT; ++x)
    {
        for (y = 0; y < SCREEN_WIDTH; ++y)
        {
            set_screen_px(screen_controller, x, y, false);
        }
    }
}

void draw_screen(ScreenController *screen_controller, bool force_draw)
{
    int page = 0;
    int column = 0;
    for (page = 0; page < PAGE_COUNT; ++page)
    {
        for (column = 0; column < SCREEN_WIDTH; ++column)
        {
            uint8_t *next_bitmap =
                    &(screen_controller->content[page][column].next_bitmap);
            uint8_t *current_bitmap =
                    &(screen_controller->content[page][column].current_bitmap);

            // Draw only when bitmap changed or forced draw
            if (force_draw || (*next_bitmap != *current_bitmap))
            {
                change_raw_screen_cursor(screen_controller, page, column,
                false);
                draw_raw_screen_page_column(*next_bitmap);
                screen_controller->content[page][column].current_bitmap =
                        *next_bitmap;
                screen_controller->raw_screen_current_column += 1; // This is done automatically by the raw screen
            }
        }
    }
}

