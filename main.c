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

    turn_physical_screen_on();

    while (1)
    {
        draw_buffer_screen_all(&screen_controller, 1);
        draw_welcome_page(&screen_controller);
        sync_screen(&screen_controller, false);

        _delay_cycles(6000000);

        draw_buffer_screen_all(&screen_controller, 1);
        draw_result_page(&screen_controller);
        sync_screen(&screen_controller, false);

        _delay_cycles(6000000);
    }
}
