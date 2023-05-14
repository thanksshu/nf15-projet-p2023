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

    turn_screen_on(&screen_controller);

    set_screen_px(&screen_controller, 31, 63, true);
    set_screen_px(&screen_controller, 10, 41, true);
    set_screen_px(&screen_controller, 38, 16, true);
    set_screen_px(&screen_controller, 20, 105, true);
    set_screen_px(&screen_controller, 20, 51, true);
    set_screen_px(&screen_controller, 47, 42, true);
    set_screen_px(&screen_controller, 20, 105, true);
    set_screen_px(&screen_controller, 16, 54, true);
    set_screen_px(&screen_controller, 43, 53, true);
    set_screen_px(&screen_controller, 16, 54, true);
    set_screen_px(&screen_controller, 16, 54, true);
    set_screen_px(&screen_controller, 31, 63, true);
    set_screen_px(&screen_controller, 30, 89, true);
    set_screen_px(&screen_controller, 0, 0, true);
    set_screen_px(&screen_controller, 63, 127, true);
    draw_screen(&screen_controller, false);

    while (1)
    {
        // Prevent exit
    }
}
