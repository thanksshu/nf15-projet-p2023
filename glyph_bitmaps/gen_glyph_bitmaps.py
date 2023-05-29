"""
Generate bitmap from "font.otf" to "glyph_bitmaps.c" and "glyph_bitmaps.h" file
"""

from PIL import Image, ImageFont, ImageDraw
import json

FONT_PATH = "glyph_bitmaps/font.otf"
CHARACTERS_PATH = "glyph_bitmaps/characters.json"
FONT_SIZE = 12

GLYPH_HEIGHT = (
    12  # Ark Pixel Font is 18 high but our screen is small, here we need to crop it
)

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


font = ImageFont.truetype(FONT_PATH, FONT_SIZE)
with open(CHARACTERS_PATH, encoding="utf-8") as chars_file:
    chars = json.load(chars_file)["characters"]
    chars.sort()
    for char in chars:
        glyph_width = font.getbbox(char)[2] - font.getbbox(char)[0]
        glyph_height = sum(font.getmetrics())

        glyph_height = GLYPH_HEIGHT  # Crop the glyph

        image = Image.new("1", (glyph_width, glyph_height), 1)
        draw = ImageDraw.Draw(image)

        # draw.text(xy=(0, 0), text=char, font=font)
        draw.text(xy=(0, -4), text=char, font=font)  # Crop the glyph

        glyph_color = [
            [image.getpixel((x, y)) for x in range(glyph_width)]
            for y in range(glyph_height)
        ]
        glyph_alpha = [
            [1 for _ in range(glyph_width)]  # Solid background
            for _ in range(glyph_height)
        ]

        bitmap_name = f"{ord(char):0>4x}"
        c_file_content.append(
            f"""
const Color u{bitmap_name}_color[{glyph_height}][{glyph_width}] = {str(glyph_color).replace("[", "{ ").replace("]", " }")};
const Alpha u{bitmap_name}_alpha[{glyph_height}][{glyph_width}] = {str(glyph_alpha).replace("[", "{ ").replace("]", " }")};
const Bitmap u{bitmap_name}_bitmap = {{ .width = {glyph_width}, .height  = {glyph_height}, .color = (Color*) u{bitmap_name}_color, .alpha = (Alpha*) u{bitmap_name}_alpha }};
"""
        )

        h_file_content.append(
            f"""
extern const Bitmap u{bitmap_name}_bitmap;
"""
        )

# Generate .c file
with open(f"glyph_bitmaps/glyph_bitmaps.c", "w") as file:
    file.write("".join(c_file_content))

# Generate .h file
h_file_content.append(
    """

#endif /* GLYPH_BITMAPS_H_ */
"""
)
with open(f"glyph_bitmaps/glyph_bitmaps.h", "w") as file:
    file.write("".join(h_file_content))
