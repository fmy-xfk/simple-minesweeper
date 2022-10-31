from machine import *
from pyb import LED

import sensor, image, time, utime, math

# 设置区域开始

#后置摄像头高光照
threshold=(10, 39, -48, 127, -128, -12)
bucket_mod=1
minx,maxx,miny,maxy=60,240,140,190

'''
#前置摄像头高光照
threshold=(15, 50, 33, 127, -128, -10)
bucket_mod=1
minx,maxx,miny,maxy=60,260,200,240

#前置摄像头低光照
threshold=(0, 29, -128, 127, -128, -22)
bucket_mod=1
minx,maxx,miny,maxy=60,260,200,240
'''

# 设置区域结束

blue = LED(3)
white = LED(4)
#white.on()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_vflip(True)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

uart=UART(2,115200)

def wr_str(string):
    for ch in string:
        uart.write(ch)
        utime.sleep_ms(1)

while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(strength = bucket_mod)
    for blob in img.find_blobs([threshold], pixels_threshold=100, area_threshold=100, merge=True, margin=10):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())

        if(minx<=blob.cx()<=maxx and miny<=blob.cy()<=maxy):
            blue.on()
            to_wr="O{},{};".format(blob.cx(),blob.cy());
            print(to_wr)
            wr_str(to_wr)
    utime.sleep_ms(100)
    blue.off()

    #print(clock.fps())
