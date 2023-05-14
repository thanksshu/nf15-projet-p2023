"""Generate unicode to bitmap mapping from BDF format font file"""

from math import ceil
from pprint import pprint


class UnicodeBitmapMap:
    def __init__(self, filename, max_unicode=0xFFFF):
        self.glyph_width = 0
        self.glyph_half_width = 0
        self.glyph_height = 0
        self.glyph_count = 0
        self.glyph_width_mask = 0b0
        self.glyph_half_width_mask = 0b0
        self.max_unicode = (
            max_unicode  # By default, only support Basic Multilingual Plane
        )

        self._bounding_box_width = 0
        self._bounding_box_height = 0
        self._bounding_box_x_offset = 0
        self._bounding_box_y_offset = 0

        self.content = []

        self._parse_file(filename)

    def _parse_file(self, filename):
        with open(filename) as file:
            self._parse_start_font(file)
            self._parse_before_properties(file)
            self._parse_properties(file)
            self._parse_chars(file)
            self._parse_bitmap_list(file)

    def _parse_start_font(self, file):
        """Read STARTFONT"""
        args = file.readline().rstrip().split(" ")
        if args[0] != "STARTFONT":
            raise SyntaxError("Not a valid BDF file")
        elif args[1] != "2.1":
            raise SyntaxError("Only BDF 2.1 is supported")

    def _parse_before_properties(self, file):
        """Before Properties"""
        while True:
            args = file.readline().rstrip().split(" ")
            match args[0]:
                case "SIZE":
                    size_in_pt = int(args[1])
                    horizontal_dpi = int(args[2])
                    vertical_dpi = int(args[3])
                    self.glyph_width = ceil(
                        size_in_pt / 72 * horizontal_dpi
                    )  # Convert pt to px
                    self.glyph_half_width = self.glyph_width // 2
                    self.glyph_width_mask = (1 << self.glyph_width) - 1
                    self.glyph_half_width_mask = (1 << self.glyph_half_width) - 1
                    self.glyph_height = ceil(size_in_pt / 72 * vertical_dpi)
                case "FONTBOUNDINGBOX":
                    width = int(args[1])
                    height = int(args[2])
                    x_offset = int(args[3])
                    y_offset = int(args[4])
                    self._bounding_box_width = width
                    self._bounding_box_height = height
                    self._bounding_box_x_offset = x_offset
                    self._bounding_box_y_offset = y_offset
                case "STARTPROPERTIES":
                    break  # End of this part
                case _:
                    continue  # Ignore things like COMMENT or COPYRIGHT

    def _parse_properties(self, file):
        """Properties"""
        while True:
            args = file.readline().rstrip().split(" ")
            match args[0]:
                case "ENDPROPERTIES":
                    break  # End of this part
                case _:
                    continue

    def _parse_chars(self, file):
        """Read CHARS"""
        args = file.readline().rstrip().split(" ")
        if args[0] != "CHARS":
            raise SyntaxError("Not a valid BDF file")
        else:
            self.glyph_count = int(args[1])

    def _parse_bitmap_list(self, file):
        """Init bitmap list"""
        self.content = [
            [0 for _ in range(self.glyph_height)] for _ in range(self.max_unicode)
        ]

        """Bitmap info"""
        unicode_index = 0x0000
        width = 0
        real_width = 0
        height = 0
        x_offset = 0
        y_offset = 0
        while True:
            args = file.readline().rstrip().split(" ")
            match args[0]:
                case "STARTCHAR":
                    if args[1].startswith("U+"):
                        unicode_index = int(args[1][2:], 16)
                    else:
                        raise SyntaxError("Only unicode format (U+0000) is supported")
                case "BBX":
                    width = int(args[1])  # Width of the bounding box, NOT the bitmap
                    # The REAL width of bitmap is always multiples of 8 (two hex digits)
                    # And the bounding box stick to the lower left corner of the bitmap
                    real_width = ceil(width / 8) * 8
                    height = int(args[2])
                    x_offset = int(args[3])
                    y_offset = int(args[4])
                case "BITMAP":
                    # In order to correctly locate pixels,
                    # we align glyph's bounding box's origin with global bounding box's origin
                    line_bitmap_horizontal_shift = (
                        self._bounding_box_x_offset + self._bounding_box_width
                    ) - (x_offset + real_width)
                    line_bitmap_vertical_shift = (
                        self._bounding_box_y_offset + self._bounding_box_height
                    ) - (y_offset + height)
                    self._parse_bitmap(
                        file,
                        unicode_index,
                        line_bitmap_horizontal_shift,
                        line_bitmap_vertical_shift,
                    )
                case "ENDFONT":
                    break
                case _:
                    pass

    def _parse_bitmap(
        self,
        file,
        unicode_index,
        line_bitmap_horizontal_shift,
        line_bitmap_vertical_shift,
    ):
        glyph_line_count = line_bitmap_vertical_shift
        while True:
            line = file.readline().rstrip()
            if line == "ENDCHAR":
                break
            if glyph_line_count > self.glyph_height - 1:
                continue  # Enough line map read
            line_bitmap = int(line, 16)  # Hex number according to specs
            if line_bitmap_horizontal_shift < 0:
                line_bitmap >>= (
                    -line_bitmap_horizontal_shift
                )  # Negative then right shift
            else:
                line_bitmap <<= line_bitmap_horizontal_shift
            self.content[unicode_index][glyph_line_count] = (
                line_bitmap & self.glyph_width_mask
            )
            glyph_line_count += 1


if __name__ == "__main__":
    # Unit test
    bitmap_list = UnicodeBitmapMap("zpix.bdf")
    pprint(bitmap_list.content)
