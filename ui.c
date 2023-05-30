#include "includes.h"

double max(double *temperatures, int length)
{
    return 37.8;
}

double min(double *temperatures, int length)
{
    return 22.6;
}

double mean(double *temperatures, int length)
{
    return 35.4;
}

Bitmap *number_to_bitmap[10] = { (Bitmap*) &u0030_bitmap,
                                 (Bitmap*) &u0031_bitmap,
                                 (Bitmap*) &u0032_bitmap,
                                 (Bitmap*) &u0033_bitmap,
                                 (Bitmap*) &u0034_bitmap,
                                 (Bitmap*) &u0035_bitmap,
                                 (Bitmap*) &u0036_bitmap,
                                 (Bitmap*) &u0037_bitmap,
                                 (Bitmap*) &u0038_bitmap,
                                 (Bitmap*) &u0039_bitmap };

Bitmap* get_digit_bitmap(double number, int digit)
{
    return number_to_bitmap[(int) (number / pow(10, digit)) % 10];
}

void draw_result_page(ScreenController *screen_controller)
{

    int index = 0;
    double temperatures[SAMPLE_COUNT] = { 0 };
    for (index = 0; index < SAMPLE_COUNT; ++index)
    {
        temperatures[index] = get_ir_sensor_temperature();
    }
    double min_temperature = min(temperatures, SAMPLE_COUNT);
    double max_temperature = max(temperatures, SAMPLE_COUNT);
    double mean_temperature = mean(temperatures, SAMPLE_COUNT);

    int line_1_length = 11;
    Bitmap *line_1_bitmaps[11] = { (Bitmap*) &u0054_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u006d_bitmap,
                                   (Bitmap*) &u0070_bitmap,
                                   (Bitmap*) &u00e9_bitmap,
                                   (Bitmap*) &u0072_bitmap,
                                   (Bitmap*) &u0061_bitmap,
                                   (Bitmap*) &u0074_bitmap,
                                   (Bitmap*) &u0075_bitmap,
                                   (Bitmap*) &u0072_bitmap,
                                   (Bitmap*) &u0065_bitmap, };
    int line_2_length = 16;
    Bitmap *line_2_bitmaps[16] = { (Bitmap*) &u004d_bitmap,
                                   (Bitmap*) &u0061_bitmap,
                                   (Bitmap*) &u0078_bitmap,
                                   (Bitmap*) &u0069_bitmap,
                                   (Bitmap*) &u006d_bitmap,
                                   (Bitmap*) &u0061_bitmap,
                                   (Bitmap*) &u006c_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u003a_bitmap, get_digit_bitmap(
                                           max_temperature, 1),
                                   get_digit_bitmap(max_temperature, 0),
                                   (Bitmap*) &u002e_bitmap, get_digit_bitmap(
                                           max_temperature, -1),
                                   get_digit_bitmap(max_temperature, -2),
                                   (Bitmap*) &u00b0_bitmap,
                                   (Bitmap*) &u0043_bitmap };

    int line_3_length = 16;
    Bitmap *line_3_bitmaps[16] = { (Bitmap*) &u004d_bitmap,
                                   (Bitmap*) &u0069_bitmap,
                                   (Bitmap*) &u006e_bitmap,
                                   (Bitmap*) &u0069_bitmap,
                                   (Bitmap*) &u006d_bitmap,
                                   (Bitmap*) &u0061_bitmap,
                                   (Bitmap*) &u006c_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u003a_bitmap, get_digit_bitmap(
                                           min_temperature, 1),
                                   get_digit_bitmap(min_temperature, 0),
                                   (Bitmap*) &u002e_bitmap, get_digit_bitmap(
                                           min_temperature, -1),
                                   get_digit_bitmap(min_temperature, -2),
                                   (Bitmap*) &u00b0_bitmap,
                                   (Bitmap*) &u0043_bitmap };
    int line_4_length = 15;
    Bitmap *line_4_bitmaps[15] = { (Bitmap*) &u004d_bitmap,
                                   (Bitmap*) &u006f_bitmap,
                                   (Bitmap*) &u0079_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u006e_bitmap,
                                   (Bitmap*) &u006e_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u003a_bitmap, get_digit_bitmap(
                                           mean_temperature, 1),
                                   get_digit_bitmap(mean_temperature, 0),
                                   (Bitmap*) &u002e_bitmap, get_digit_bitmap(
                                           mean_temperature, -1),
                                   get_digit_bitmap(mean_temperature, -2),
                                   (Bitmap*) &u00b0_bitmap,
                                   (Bitmap*) &u0043_bitmap };

    draw_buffer_screen_bitmap(screen_controller, 0, 0,
                              (Bitmap*) &background_bitmap);

    draw_buffer_screen_bitmaps_on_row(screen_controller, 2 + 0 * GLYPH_HEIGHT,
                                      2, line_1_bitmaps, line_1_length);

    draw_buffer_screen_bitmaps_on_row(screen_controller, 2 + 1 * GLYPH_HEIGHT,
                                      2, line_2_bitmaps, line_2_length);

    draw_buffer_screen_bitmaps_on_row(screen_controller, 2 + 2 * GLYPH_HEIGHT,
                                      2, line_3_bitmaps, line_3_length);

    draw_buffer_screen_bitmaps_on_row(screen_controller, 2 + 3 * GLYPH_HEIGHT,
                                      2, line_4_bitmaps, line_4_length);

}

void draw_welcome_page(ScreenController *screen_controller)
{
    int line_1_length = 8;
    Bitmap *line_1_bitmaps[8] = { (Bitmap*) &u0042_bitmap,
                                  (Bitmap*) &u006f_bitmap,
                                  (Bitmap*) &u006e_bitmap,
                                  (Bitmap*) &u006a_bitmap,
                                  (Bitmap*) &u006f_bitmap,
                                  (Bitmap*) &u0075_bitmap,
                                  (Bitmap*) &u0072_bitmap,
                                  (Bitmap*) &u002c_bitmap };
    int line_2_length = 14;
    Bitmap *line_2_bitmaps[14] = { (Bitmap*) &u0075_bitmap,
                                   (Bitmap*) &u0074_bitmap,
                                   (Bitmap*) &u0069_bitmap,
                                   (Bitmap*) &u006c_bitmap,
                                   (Bitmap*) &u0069_bitmap,
                                   (Bitmap*) &u0073_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u007a_bitmap,
                                   (Bitmap*) &u0020_bitmap,
                                   (Bitmap*) &u0052_bitmap,
                                   (Bitmap*) &u0049_bitmap,
                                   (Bitmap*) &u0047_bitmap,
                                   (Bitmap*) &u0048_bitmap,
                                   (Bitmap*) &u0054_bitmap };
    int line_3_length = 12;
    Bitmap *line_3_bitmaps[12] = { (Bitmap*) &u0070_bitmap,
                                   (Bitmap*) &u006f_bitmap,
                                   (Bitmap*) &u0075_bitmap,
                                   (Bitmap*) &u0072_bitmap,
                                   (Bitmap*) &u0020_bitmap,
                                   (Bitmap*) &u006d_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u0073_bitmap,
                                   (Bitmap*) &u0075_bitmap,
                                   (Bitmap*) &u0072_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u0072_bitmap };

    int line_4_length = 15;
    Bitmap *line_4_bitmaps[15] = { (Bitmap*) &u006c_bitmap,
                                   (Bitmap*) &u0061_bitmap,
                                   (Bitmap*) &u0020_bitmap,
                                   (Bitmap*) &u0074_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u006d_bitmap,
                                   (Bitmap*) &u0070_bitmap,
                                   (Bitmap*) &u00e9_bitmap,
                                   (Bitmap*) &u0072_bitmap,
                                   (Bitmap*) &u0061_bitmap,
                                   (Bitmap*) &u0074_bitmap,
                                   (Bitmap*) &u0075_bitmap,
                                   (Bitmap*) &u0072_bitmap,
                                   (Bitmap*) &u0065_bitmap,
                                   (Bitmap*) &u002e_bitmap };

    draw_buffer_screen_bitmap(screen_controller, 0, 0,
                              (Bitmap*) &background_bitmap);

    draw_buffer_screen_bitmaps_on_row(screen_controller, 2 + 0 * GLYPH_HEIGHT,
                                      2, line_1_bitmaps, line_1_length);

    draw_buffer_screen_bitmaps_on_row(screen_controller, 2 + 1 * GLYPH_HEIGHT,
                                      2, line_2_bitmaps, line_2_length);

    draw_buffer_screen_bitmaps_on_row(screen_controller, 2 + 2 * GLYPH_HEIGHT,
                                      2, line_3_bitmaps, line_3_length);

    draw_buffer_screen_bitmaps_on_row(screen_controller, 2 + 3 * GLYPH_HEIGHT,
                                      2, line_4_bitmaps, line_4_length);

}
