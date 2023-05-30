/*
 * Main executable
 */

#include "includes.h"

bool is_right_pressed()
{
    return true;
}

bool is_left_pressed()
{
    return true;
}

bool is_val_pressed()
{
    return true;
}

int main()
{
    /*
     * Initialisation
     */
    // Stop WatchDog
    MAP_WDT_A_holdTimer();

    // Set SMCLK at 12MHz and MCLK at 1.5MHz
    CS_initClockSignal(CS_SMCLK, CS_MODOSC_SELECT, CS_CLOCK_DIVIDER_2);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);

    // I2C
    init_ir_temperature_sensor_communication();

    // Screen
    ScreenController screen_controller = new_screen_controller();

    // UI
    enum page current_page;
    current_page = Off;

    // Interruption
    __enable_irq();

    /*
     * Main loop
     */
    while (1)
    {
        _delay_cycles(300000); // TODO: Remove test code

        // Turn on
        if (is_val_pressed() && current_page == Off)
        {
            turn_physical_screen_on();

            // Enter welcome screen when powered on
            draw_buffer_screen_all(&screen_controller, CLEAR_SCREEN_COLOR);
            draw_welcome_page(&screen_controller);
            sync_screen(&screen_controller, false);
            current_page = Welcome;
        }

        // From welcome to result
        if (is_right_pressed() && (current_page == Welcome))
        {
            // Loading screen
            draw_buffer_screen_all(&screen_controller, CLEAR_SCREEN_COLOR);
            draw_buffer_screen_bitmap(&screen_controller, 0, 0,
                                      (Bitmap*) &background_bitmap);
            draw_buffer_screen_bitmap(&screen_controller, 0, 0,
                                      (Bitmap*) &loading_bitmap);
            sync_screen(&screen_controller, false);

            // Result
            draw_buffer_screen_all(&screen_controller, CLEAR_SCREEN_COLOR);
            draw_result_page(&screen_controller);
            sync_screen(&screen_controller, false);
            current_page = Result;
        }

        // From result to welcome
        if (is_left_pressed() && (current_page == Result))
        {
            draw_buffer_screen_all(&screen_controller, CLEAR_SCREEN_COLOR);
            draw_welcome_page(&screen_controller);
            sync_screen(&screen_controller, false);
            current_page = Welcome;
        }

        // Turn off
        if (is_val_pressed() && current_page != Off)
        {
            turn_physical_screen_off();
            current_page = Off;
        }
    }
}
