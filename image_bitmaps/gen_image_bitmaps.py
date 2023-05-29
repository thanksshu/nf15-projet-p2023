"""
Generate bitmap from "piskels/*.piskel" to "{name}.c" and update "ui_bitmaps.h" file
Note: Only the first frame of the first chunk of the first layer will be converted
Note: Only white, black, transparent three pixels are supported
"""
import json
from base64 import b64decode
from io import BytesIO
from os import walk

from PIL import Image

PISKEL_DIR = "image_bitmaps/piskels"

names = []
for filename in next(walk(PISKEL_DIR), (None, None, []))[2]:  # [] if no file
    """Read piskel file and extract useful infomation"""
    if not filename.endswith(".piskel") :
        continue
    with open(f"{PISKEL_DIR}/{filename}") as file:
        model_version, content = json.load(file).values()

        if model_version != 2:
            raise NotImplementedError("Only model version 2 is supported")

        name = content["name"]
        names.append(name)
        description = content["description"]
        width = content["width"]
        height = content["height"]

        layer = json.loads(content["layers"][0])
        chunk = layer["chunks"][0]
        base64_image = chunk["base64PNG"].replace("data:image/png;base64,", "")
        image = Image.open(BytesIO(b64decode(base64_image)))
        image_rgba = [
            [image.getpixel((x, y)) for x in range(width)] for y in range(height)
        ]

        bitmap_color = [
            [
                1
                if (pixel[0] > 127 or pixel[1] > 127 or pixel[2] > 127)
                else 0  # RGB channel "looks like" white
                for pixel in row
            ]
            for row in image_rgba
        ]
        bitmap_alpha = [
            [
                1 if (pixel[3] > 127) else 0 for pixel in row
            ]  # Alpha channel "looks like" solid
            for row in image_rgba
        ]

    """Generate .c files"""

    c_file_content = f"""/*
 * {description}
 */
#include "../includes.h"

const Color {name}_color[{height}][{width}] = {str(bitmap_color).replace("[", "{ ").replace("]", " }")};
const Alpha {name}_alpha[{height}][{width}] = {str(bitmap_alpha).replace("[", "{ ").replace("]", " }")};
const Bitmap {name}_bitmap = {{ .height = {height}, 
                                .width = {width}, 
                                .color = (Color*) {name}_color, 
                                .alpha = (Alpha*) {name}_alpha }};
"""

    with open(f"image_bitmaps/{name}_bitmap.c", "w") as file:
        file.write(c_file_content)

"""Generate ui_bitmaps.h"""
h_file_content = "".join(
    [
        """/*
 * Contents predefine bitmaps used in UI
 */
#include "../includes.h"

#ifndef IMAGE_BITMAPS_H_
#define IMAGE_BITMAPS_H_

""",
        *[f"extern const Bitmap {name}_bitmap;\n" for name in names],
        """
#endif /* IMAGE_BITMAPS_H_ */
""",
    ]
)

with open(f"image_bitmaps/image_bitmaps.h", "w") as file:
    file.write(h_file_content)
