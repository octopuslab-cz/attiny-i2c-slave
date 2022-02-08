import time

def send_wsled(i2c, led, r, g, b):
    tmp = bytearray(5)
    tmp[0] = 0x05
    tmp[1] = led
    tmp[2] = r
    tmp[3] = g
    tmp[4] = b
    i2c.writeto(0x14, tmp)

def send_wsrainbow(i2c, hue):
    tmp = bytearray(3)
    tmp[0] = 0x06
    tmp[1] = hue >> 8
    tmp[2] = hue & 0xff
    i2c.writeto(0x14, tmp)


def test_singleled():
    sleep = 10
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


def test_hue():
    hue = 0
    while True:
        hue+=2048
        send_wsrainbow(i, hue)
        time.sleep_ms(50)

    

o = octopus()
i = o.i2c_init()
