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

#ifndef UI_GLYPHS_UI_GLYPHS_H_
#define UI_GLYPHS_UI_GLYPHS_H_

"""
)

with open(CHARACTERS_FILENAME, encoding="utf-8") as chars_file:
    chars = json.load(chars_file)["characters"]
    chars.sort()
    for char in chars:
        unicode = ord(char)
        glyph_width = bitmap_font.glyphs[unicode]["width"]
        bitmap_str = ", ".join(
            f"0x{bitmap:0>4x}" for bitmap in bitmap_font.glyphs[unicode]["bitmap"]
        )

        c_file_content.append(
            f"""const Glyph u{unicode:0>4x} = {{ .width = {glyph_width}, .bitmap = {{ {bitmap_str} }} }};

"""
        )

        h_file_content.append(
            f"""extern const Glyph u{unicode:0>4x};
"""
        )

# Generate .c file
with open(f"ui_glyphs.c", "w") as file:
    file.write("".join(c_file_content))

# Generate .h file
h_file_content.append(
    """
#endif /* UI_GLYPHS_UI_GLYPHS_H_ */
"""
)
with open(f"ui_glyphs.h", "w") as file:
    file.write("".join(h_file_content))
