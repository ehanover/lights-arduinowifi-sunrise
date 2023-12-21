# Arduino sunrise lamp

## POST binary data format
- Byte 0: Mode identifier
  - Case `0`: Sunrise
    - Byte 1,2,3,4: Seconds till alarm, as int (4 bytes)
    - Byte 5,6,7: RGB of target color
    - Byte 8: Brightness
    - Byte 9: Minute duration of sunrise
  - Case `1`: Fade
    - Byte 1,2,3: RGB of color 1
    - Byte 4,5,6: RGB of color 2
    - Byte 7,8,9: RGB of color 3
    - Byte 10: Speed
