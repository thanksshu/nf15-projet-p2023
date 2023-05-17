/*
 * Main executable
 */

#include "includes.h"

int main()
{
    // Stop WatchDog
    MAP_WDT_A_holdTimer();

    // Initialise SMCLK at 12MHz
    CS_initClockSignal(CS_SMCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_2);

    ScreenController screen_controller = new_screen_controller();
    set_buffer_screen_display_on(&screen_controller, true);

    set_buffer_screen_bitmap(&screen_controller, 0, 0, (Bitmap*) &test_bitmap);

    sync_screen(&screen_controller, false);

    while (1)
    {

    }
}
