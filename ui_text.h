/*
 * UI text to bitmap convention
 */

#ifndef UI_TEXT_H_
#define UI_TEXT_H_

typedef struct {
    int width; // Width of a glyph's bounding box
    uint16_t bitmap[GLYPH_HEIGHT];
} Glyph;

#endif /* UI_TEXT_H_ */
