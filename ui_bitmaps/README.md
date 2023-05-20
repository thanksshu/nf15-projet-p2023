# Generate predefined bitmap used in program

- Use [piskel](https://www.piskelapp.com/) to draw bitmap art, but there's some limitations :
  - Only the first layer of the first frame will be converted
  - Only support black, white and transparent pixels
  - Output file name depends on the name of the drawing, so you SHOULD use pascal case to make me (and the script) happy
- Download the finished bitmap as "\*.piskel" file, put in to "piskels/" directory. "piskels/" MUST ONLY contain "\*.piskel".
- Run the "gen_ui_bitmaps.py" script in this directory. Note that python package `Pillow` is needed

The "gen_ui_bitmaps.py" will automatically generate "*.c" files and generate "ui_bitmaps.h", currently only support Windows.
