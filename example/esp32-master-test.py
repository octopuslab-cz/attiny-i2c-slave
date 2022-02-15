import time

def send_wsled(i2c, led, r, g, b):
    tmp = bytearray(6)
    tmp[0] = 0x05
    tmp[1] = 0x05
    tmp[2] = led
    tmp[3] = r
    tmp[4] = g
    tmp[5] = b
    i2c.writeto(0x14, tmp)


def send_wsrainbow(i2c, hue):
    tmp = bytearray(4)
    tmp[0] = 0x05
    tmp[1] = 0x06
    tmp[2] = hue >> 8
    tmp[3] = hue & 0xff
    i2c.writeto(0x14, tmp)


def set_ws_num(i2c, num):
    tmp = bytearray(3)
    tmp[0] = 0x05
    tmp[1] = 0x03
    tmp[2] = num
    i2c.writeto(0x14, tmp)


def set_ws_brightness(i2c, br):
    tmp = bytearray(3)
    tmp[0] = 0x05
    tmp[1] = 0x04
    tmp[2] = br
    i2c.writeto(0x14, tmp)
    
    
def fill_wsled(i2c, start, count, r, g, b):
    tmp = bytearray(7)
    tmp[0] = 0x05
    tmp[1] = 0x08
    tmp[2] = start
    tmp[3] = count
    tmp[4] = r
    tmp[5] = g
    tmp[6] = b
    i2c.writeto(0x14, tmp)


def test_singleled(sleep=10):
    while True:
        send_wsled(i, 0, 0,255,0)
        send_wsled(i, 1, 0,0,0)
        send_wsled(i, 2, 0,0,0)
        send_wsled(i, 3, 0,0,0)
        time.sleep_ms(sleep)
        send_wsled(i, 0, 0,0,0)
        send_wsled(i, 1, 0,255,0)
        send_wsled(i, 2, 0,0,0)
        send_wsled(i, 3, 0,0,0)
        time.sleep_ms(sleep)
        send_wsled(i, 0, 0,0,0)
        send_wsled(i, 1, 0,0,0)
        send_wsled(i, 2, 0,255,0)
        send_wsled(i, 3, 0,0,0)
        time.sleep_ms(sleep)
        send_wsled(i, 0, 0,0,0)
        send_wsled(i, 1, 0,0,0)
        send_wsled(i, 2, 0,0,0)
        send_wsled(i, 3, 0,255,0)
        time.sleep_ms(sleep)


def test_hue(delay=50):
    hue = 0
    while True:
        hue+=2048
        send_wsrainbow(i, hue)
        time.sleep_ms(delay)

def test_fade(fr=1, to=50, delay=50, inc=5):
    for br in range(fr, to, inc):
        set_ws_brightness(i, br)
        time.sleep_ms(delay)


o = octopus()
i = o.i2c_init()


while True:
    test_fade(1,100,0, inc=15)
    test_fade(100,1,0, inc=-15)
