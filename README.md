# Arduino sunrise lamp

## POST binary data format
- Byte 0: Mode identifier
  - Case `0`: Off
  - Case `1`: Solid
    - Byte 1,2,3: RGB of color
  - Case `2`: Sunrise
    - Byte 1,2,3: Seconds till alarm, interpreted as 32-bit long on Arduino (without top byte), largest byte first
    - Byte 4,5,6: RGB of target color
    - Byte 7: Minute duration of sunrise
  - Case `3`: Swirl
    - Byte 1,2,3: Hues of 3 colors
    - Byte 4: Speed
    - Byte 5: Brightness
  - Case `4`: Rave
    - Byte 1: `0`=flashing colors, `1`=rainbow
    - Byte 2: Speed
    - Byte 3: Brightness
  - Case `5`: Fireworks
    - Byte 1: Speed
    - Byte 2: Brightness
  - Case `6`: Siren
    - Byte 1,2,3: RGB of color 1
    - Byte 4,5,6: RGB of color 2
    - Byte 7: Speed

## Ideas
- Rave: random sequence of colors (or add random skips in the sequence)
- Swirl: color preset links on the UI
- Sunrise: single dim pixel to show status during darkness
- Optimize serving webpage with fetch() or iframe
- New mode where pixels individually switch randomly