/*
 * UI components
 */

#ifndef UI_H_
#define UI_H_

enum page
{
    Welcome, Result, Off
};

void draw_welcome_page(ScreenController *screen_controller);
void draw_result_page(ScreenController *screen_controller);

#endif /* UI_H_ */
