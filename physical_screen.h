/*
 * SPI commands to communicate with the physical screen
 */

#ifndef PHYSICAL_SCREEN_H_
#define PHYSICAL_SCREEN_H_

void init_raw_screen_communication();
void command_raw_screen(uint8_t data, bool mode_write);
void init_raw_screen_display();
void turn_raw_screen_on();
void turn_raw_screen_off();
void set_raw_screen_page(int page);
void set_raw_screen_column(int column);
void draw_raw_screen_page_column(uint8_t bitmap);

#endif /* PHYSICAL_SCREEN_H_ */
