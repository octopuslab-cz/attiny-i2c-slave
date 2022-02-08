# AT Tiny 85 I2C Slave


Supported write registers

0x05 - Set WS Led single color
  - uint8 - LED number
  - uint8 - Red
  - uint8 - Green
  - uint8 - Blue

0x06 - Set WS Led rainbow starting hue
  - uint16 - Hue

