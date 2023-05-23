"""Convert a BDF font file to glyph bitmaps"""

from math import ceil
from pprint import pprint


class BitmapFont:
    def __init__(self, filename, max_unicode=0xFFFF):
        # Glyph
        self.glyph_full_width = 0
        self.glyph_half_width = 0
        self.glyph_height = 0

        # Global bounding box,
        # The bounding box's origin stick to the lower left corner of the bitmap
        self.bounding_box_width = 0
        self.bounding_box_height = 0

        # Offset from the lower left corner of the bitmap
        self.bounding_box_x_offset = (
            0  # X axis along width of the bitmap, from left to right
        )
        self.bounding_box_y_offset = (
            0  # Y axis along height of the bitmap, from bottom to top
        )

        self.glyph_count = 0

        self.glyph_width_mask = 0b0
        self.glyph_half_width_mask = 0b0

        self.max_unicode = (
            max_unicode  # By default, only support Basic Multilingual Plane
        )
        self.glyphs = []
        self._parse_file(filename)

    def _parse_file(self, filename):
        with open(filename) as file:
            self._parse_start_font(file)
            self._parse_before_properties(file)
            self._parse_properties(file)
            self._parse_chars(file)
            self._parse_glyph_list(file)

    def _parse_start_font(self, file):
        """Read STARTFONT"""
        args = file.readline().rstrip().split(" ")
        if args[0] != "STARTFONT":
            raise SyntaxError("Not a valid BDF file")
        elif args[1] != "2.1":
            raise NotImplementedError("Only BDF 2.1 is supported")

    def _parse_before_properties(self, file):
        """Before Properties"""
        while True:
            args = file.readline().rstrip().split(" ")
            match args[0]:
                case "SIZE":
                    size_in_pt = int(args[1])  # The "pt" unit is for printing
                    horizontal_dpi = int(args[2])
                    vertical_dpi = int(args[3])
                    self.glyph_full_width = ceil(
                        size_in_pt / 72 * horizontal_dpi
                    )  # Convert pt to px
                    self.glyph_half_width = self.glyph_full_width // 2
                    self.glyph_width_mask = (1 << self.glyph_full_width) - 1
                    self.glyph_half_width_mask = (1 << self.glyph_half_width) - 1
                    self.glyph_height = ceil(size_in_pt / 72 * vertical_dpi)
                case "FONTBOUNDINGBOX":
                    self.bounding_box_width = int(args[1])
                    self.bounding_box_height = int(args[2])
                    self.bounding_box_x_offset = int(args[3])
                    self.bounding_box_y_offset = int(args[4])
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
                    continue  # Ignore all properties

    def _parse_chars(self, file):
        """Read CHARS"""
        args = file.readline().rstrip().split(" ")
        if args[0] != "CHARS":
            raise SyntaxError("Not a valid BDF file")
        else:
            self.glyph_count = int(args[1])

    def _parse_glyph_list(self, file):
        """Init glyph bitmap list"""
        self.glyphs = [
            {"width": 0, "bitmap": [0 for _ in range(self.glyph_height)]}
            for _ in range(self.max_unicode)
        ]

        """Bitmap info"""
        unicode_index = 0x0000
        glyph_width = 0
        bitmap_width = 0
        glyph_height = 0
        glyph_bounding_box_x_offset = 0
        glyph_bounding_box_y_offset = 0
        while True:
            args = file.readline().rstrip().split(" ")
            match args[0]:
                case "STARTCHAR":
                    if args[1].startswith("U+"):
                        unicode_index = int(args[1][2:], 16)
                    else:
                        raise NotImplementedError(
                            "Only unicode format (U+0000) is supported"
                        )
                case "BBX":
                    glyph_width = int(
                        args[1]
                    )  # Width of the GLYPH BOUNDING BOX, NOT THE BITMAP
                    self.glyphs[unicode_index]["width"] = glyph_width
                    # The width OF A BITMAP is always multiples of 8 (two hex digits)
                    # And the bounding box's origin stick to the lower left corner of the bitmap
                    bitmap_width = ceil(glyph_width / 8) * 8
                    glyph_height = int(args[2])
                    glyph_bounding_box_x_offset = int(args[3])
                    glyph_bounding_box_y_offset = int(args[4])
                case "BITMAP":
                    # In order to correctly locate pixels,
                    # we align glyph bounding box's origin with global bounding box's origin
                    # by performing a shift
                    bitmap_horizontal_shift = (
                        self.bounding_box_x_offset + self.bounding_box_width
                    ) - (glyph_bounding_box_x_offset + bitmap_width)
                    bitmap_vertical_shift = (
                        self.bounding_box_y_offset + self.bounding_box_height
                    ) - (glyph_bounding_box_y_offset + glyph_height)
                    self._parse_bitmap(
                        file,
                        unicode_index,
                        bitmap_horizontal_shift,
                        bitmap_vertical_shift,
                    )
                case "ENDFONT":
                    break
                case _:
                    pass

    def _parse_bitmap(
        self,
        file,
        unicode_index,
        bitmap_horizontal_shift,
        bitmap_vertical_shift,
    ):
        glyph_row_count = bitmap_vertical_shift  # Start with a shift
        while True:
            line = file.readline().rstrip()
            if line == "ENDCHAR":
                break
            if glyph_row_count > self.glyph_height - 1:
                continue  # Enough row has been read, ignore the rest

            row_bitmap = int(line, 16)  # A line is actually a hex number
            if bitmap_horizontal_shift < 0:
                row_bitmap >>= -bitmap_horizontal_shift  # Negative, so shift right
            else:
                row_bitmap <<= bitmap_horizontal_shift
            self.glyphs[unicode_index]["bitmap"][glyph_row_count] = (
                row_bitmap & self.glyph_width_mask
            )
            glyph_row_count += 1  # Next row


if __name__ == "__main__":
    # Test
    bitmap_font = BitmapFont("test.bdf", max_unicode=3)
    for glyph in bitmap_font.glyphs:
        print(f"width : {glyph['width']}")
        for row in glyph["bitmap"]:
            print(f"{row:0>{bitmap_font.glyph_full_width}b}".replace("1", "█"))
        print()
