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
    // Initialise MCLK at 1.5MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);

    ScreenController screen_controller = new_screen_controller();

    turn_screen_on_off(&screen_controller, true);

    sync_screen(&screen_controller, false);

    while (1)
    {
    }
}
