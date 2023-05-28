"""
Generate bitmap from "piskels/*.piskel" to "{name}.c" and update "ui_bitmaps.h" file
Note: Only the first frame of the first chunk of the first layer will be converted
Note: Only white, black, transparent three pixels are supported
"""
from read_font import BitmapFont
import json

FONT_FILENAME = "font.bdf"
CHARACTERS_FILENAME = "characters.json"
bitmap_font = BitmapFont(FONT_FILENAME)

c_file_content = []
h_file_content = []

c_file_content.append(
    """#include "../includes.h"

"""
)

h_file_content.append(
    """/*
 * Contents glyph bitmaps used in UI
 */
#include "../includes.h"

#ifndef GLYPH_BITMAPS_H_
#define GLYPH_BITMAPS_H_
"""
)

with open(CHARACTERS_FILENAME, encoding="utf-8") as chars_file:
    chars = json.load(chars_file)["characters"]
    chars.sort()
    for char in chars:
        unicode = ord(char)
        glyph_width = bitmap_font.glyphs[unicode]["width"]
        glyph_height = bitmap_font.glyph_height
        glyph_color = [
            [
                0 if digit == "1" else 1  # Black if this bit need to be draw
                for digit in f"{bitmap:0>{bitmap_font.glyph_full_width}b}"[:glyph_width]
            ]
            for bitmap in bitmap_font.glyphs[unicode]["bitmap"]
        ]
        glyph_alpha = [
            [1 for _ in range(glyph_width)]  # Solid background
            for _ in range(glyph_height)
        ]

        c_file_content.append(
            f"""
const Color u{unicode:0>4x}_color[{glyph_height}][{glyph_width}] = {str(glyph_color).replace("[", "{ ").replace("]", " }")};
const Alpha u{unicode:0>4x}_alpha[{glyph_height}][{glyph_width}] = {str(glyph_alpha).replace("[", "{ ").replace("]", " }")};
const Bitmap u{unicode:0>4x}_bitmap = {{ .width = {glyph_width}, .height  = {glyph_height}, .color = (Color*) u{unicode:0>4x}_color, .alpha = (Alpha*) u{unicode:0>4x}_alpha }};
"""
        )

        h_file_content.append(
            f"""
extern const Bitmap u{unicode:0>4x}_bitmap;
"""
        )

# Generate .c file
with open(f"glyph_bitmaps.c", "w") as file:
    file.write("".join(c_file_content))

# Generate .h file
h_file_content.append(
    """
extern const Bitmap u0020_bitmap;
    
#endif /* GLYPH_BITMAPS_H_ */
"""
)
with open(f"glyph_bitmaps.h", "w") as file:
    file.write("".join(h_file_content))
