# BioSensor Dev

This is the code base for BioSensor Dev (NF15 2023P).

## Generate glyph bitmaps used in program

We used [Ark Pixel Font](https://github.com/TakWolf/ark-pixel-font) for bitmap generation

- Put the needed font file in `glyph_bitmaps` and rename it to `font.otf`
- Edit the `glyph_bitmaps/characters.json`, add needed characters
- Run the `glyph_bitmaps/gen_glyph_bitmapas.py`. Note that python package `Pillow` is needed and we currently only support Windows

The "gen_glyph_bitmaps.py" will automatically generate `glyph_bitmaps.c` and `glyph_bitmaps.h` in `glyph_bitmaps`

If a sequence of glyph bitmap is needed, a helper script can be used:

```python
input_phrase = input("")
for character in input_phrase:
    print(f"(Bitmap*) &u{ord(character):0>4x}_bitmap,")
print(f"Length: {index + 1}")
```

## Generate image bitmaps used in program

- Use [piskel](https://www.piskelapp.com/) to draw bitmap art, but there are some limitations :
  - Only the first layer of the first frame will be converted
  - Only support black, white and transparent pixels
  - Output file name depends on the name of the drawing, so you SHOULD use pascal case to make me (and the script) happy
- Download the finished bitmap as `*.piskel` file, put it into `image_bitmaps/piskels`
- Run `image_bitmaps/gen_ui_bitmaps.py`. Note that python package `Pillow` is needed and we currently only support Windows

The "gen_ui_bitmaps.py" will automatically generate `*_bitmap.c` files and generate `ui_bitmaps.h` in `image_bitmaps`
