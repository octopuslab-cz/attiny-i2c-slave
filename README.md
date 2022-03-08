# AT Tiny 85 I2C Slave

16bit memory addressing

1st byte - register
2nd byte - subregister


## Supported read registers
0x01 - System register


## Supported write registers
0x05 - WS Led register


### System register - read
0x01 - Firmware version
  - return 3 bytes
  - uint_8 - Major version
  - uint_8 - Minor version
  - uint_8 - Patch version


### WS Led register - write
0x03 - Set number of led in strip
  - uint8 - Number of leds

0x04 - Set brightness
  - uint8 - 0 (black) - 255 (Max)

0x05 - Set WS Led single color
  - uint8 - LED number
  - uint8 - Red
  - uint8 - Green
  - uint8 - Blue

0x06 - Set rainbow
  - uint16 - 16bit HUE color

0x08 - Fill led by single color
  - uint8 - Start led
  - uint8 - Count
  - uint8 - Red
  - uint8 - Green
  - uint8 - Blue
