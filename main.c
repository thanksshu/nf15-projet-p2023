/*
 * main.c
 *
 * Main executable
 */

#include "includes.h"

int main(void)
{
    /* stop watchdog  */
    MAP_WDT_A_holdTimer();

    /* Initialise screen */
    RawScreen raw_screen;
    PageBlockRowMap PAGE_BLOCK_ROW_MAP = new_page_block_row_map(); // initial page block row map as a global constant
    CharScreen char_screen = new_char_screen();

    // Test char screen
    // TODO: RawScreen not finished
    clear_char_screen(&char_screen);
    draw_char_screen(&char_screen, &PAGE_BLOCK_ROW_MAP,
                     &BITMAP_MAP, &raw_screen);
    printf("------\n");

    set_char_block(&char_screen, 0, 0, 0x0, Left);
    set_char_block(&char_screen, 0, 1, 0x1, Right);
    set_char_block(&char_screen, 0, 2, 0x0, Left);
    set_char_block(&char_screen, 0, 3, 0x1, Left);
    draw_char_screen(&char_screen, &PAGE_BLOCK_ROW_MAP,
                     &BITMAP_MAP, &raw_screen);
    printf("------\n");

    clear_char_screen(&char_screen);
    draw_char_screen(&char_screen, &PAGE_BLOCK_ROW_MAP,
                     &BITMAP_MAP, &raw_screen);
    printf("------\n");

    set_char_block(&char_screen, 2, 0, 0x0, Left);
    set_char_block(&char_screen, 2, 0, 0x1, Left);
    set_char_block(&char_screen, 2, 0, 0x0, Left);
    set_char_block(&char_screen, 2, 1, 0x1, Left);
    set_char_block(&char_screen, 2, 2, 0x1, Left);
    set_char_block(&char_screen, 2, 3, 0x1, Right);
    set_char_block(&char_screen, 2, 4, 0x1, Left);
    draw_char_screen(&char_screen, &PAGE_BLOCK_ROW_MAP,
                     &BITMAP_MAP, &raw_screen);
    printf("------\n");

    while (1)
    {
    }
}
