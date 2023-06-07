/*
 * Main executable
 */

#include "includes.h"

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

    // Button
    init_button();

    // I2C
    init_ir_temperature_sensor_communication();

    // Screen
    ScreenController screen_controller = new_screen_controller();

    // UI
    enum page current_page;
    current_page = Off;

    // Interruption
    __enable_irq();

    // Set the emissivity of the IR sensor
    // printf("read: %f\n", get_ir_sensor_emissivity());
    // printf("write: %d\n", set_ir_sensor_emissivity(0.97));
    // printf("read: %f\n", get_ir_sensor_emissivity());
    // printf("write: %d\n", set_ir_sensor_emissivity(0.97));
    // printf("read: %f\n", get_ir_sensor_emissivity());

    /*
     * Main loop
     */
    while (1)
    {
        // Turn on
        if ((current_page == Off) && is_val_pressed())
        {
            // Enter welcome screen when powered on
            draw_buffer_screen_all(&screen_controller, CLEAR_SCREEN_COLOR);
            draw_welcome_page(&screen_controller);
            sync_screen(&screen_controller, false);
            turn_physical_screen_on();
            current_page = Welcome;
        }

        // From welcome to result
        if ((current_page == Welcome) && is_right_pressed())
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
        if ((current_page == Result) && is_left_pressed())
        {
            draw_buffer_screen_all(&screen_controller, CLEAR_SCREEN_COLOR);
            draw_welcome_page(&screen_controller);
            sync_screen(&screen_controller, false);
            current_page = Welcome;
        }

        // Turn off
        if (current_page != Off && is_val_pressed())
        {
            turn_physical_screen_off();
            current_page = Off;
        }
    }
}
