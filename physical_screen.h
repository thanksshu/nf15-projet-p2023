/*
 * SPI commands to communicate with the physical screen
 */

#ifndef PHYSICAL_SCREEN_H_
#define PHYSICAL_SCREEN_H_

void init_physical_screen_communication();
void command_physical_screen(uint8_t data, bool mode_write);
void init_physical_screen_display();
void turn_physical_screen_on();
void turn_physical_screen_off();
void set_physical_screen_page(int page);
void set_physical_screen_column(int column);
void draw_physical_screen_page_column(uint8_t bitmap);

#endif /* PHYSICAL_SCREEN_H_ */
