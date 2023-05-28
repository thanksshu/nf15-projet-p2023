# Generate glyph bitmaps used in program

- Put the needed font file in this directory and rename to "font.bdf"
- Edit the "characters.txt", add needed characters. Note that the file must be UTF-8 and 0 width glyphs will cause error when compiling the C code, so remove them
- Run the "gen_ui_bitmaps.py" script in this directory

The "gen_ui_glyphs.py" will automatically generate "ui_glyphs.c" files and generate "ui_glyphs.h"

If a sequence of glyph bitmap is needed, a helper script can be used:

```python
input_phrase = input("")
for character in input_phrase:
    print(f"(Bitmap*) &u{ord(character):0>4x}_bitmap,")
```
