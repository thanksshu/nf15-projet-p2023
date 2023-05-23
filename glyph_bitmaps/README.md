# Generate glyph bitmaps used in program

- Put the needed font file in this directory and rename to "font.bdf"
- Edit the "characters.txt", add needed characters. Note that the file must be UTF-8
- Run the "gen_ui_bitmaps.py" script in this directory

The "gen_ui_glyphs.py" will automatically generate "ui_glyphs.c" files and generate "ui_glyphs.h"
