/*
 * SPI commands to communicate with the "physical screen" (raw screen)
 */

#ifndef RAW_SCREEN_H_
#define RAW_SCREEN_H_

void init_raw_screen_communication();
void command_raw_screen(uint8_t data, bool mode_write);
void init_raw_screen_display();
void turn_raw_screen_on();
void turn_raw_screen_off();
void select_raw_screen_page(int page);
void select_raw_screen_column(int column);
void draw_raw_screen_page_column(uint8_t bitmap);

#endif /* RAW_SCREEN_H_ */
