from machine import *
from pyb import LED

import sensor, image, time, utime, math

# Region of settings begins!

# Back camera in high-light-intensity area
threshold=(10, 39, -48, 127, -128, -12)
bucket_mod=1
minx,maxx,miny,maxy=60,240,140,190

'''
# Front camera in high-light-intensity area
threshold=(15, 50, 33, 127, -128, -10)
bucket_mod=1
minx,maxx,miny,maxy=60,260,200,240

# Front camera in low-light-intensity area
threshold=(0, 29, -128, 127, -128, -22)
bucket_mod=1
minx,maxx,miny,maxy=60,260,200,240
'''

# Region of settings ends!

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

# Write date to the serial port
def wr_str(string):
    for ch in string:
        uart.write(ch)
        utime.sleep_ms(1)

while(True):
    clock.tick()

    # Get the image and correct its distortion
    img = sensor.snapshot().lens_corr(strength = bucket_mod)

    # Filter the blue pixels by thresold
    for blob in img.find_blobs([threshold], pixels_threshold=100, area_threshold=100, merge=True, margin=10):
        # Mark the blue circle found
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())

        # When the blue circle is located in given area determined by minx,maxx,miny,maxy, a mine is found
        if(minx<=blob.cx()<=maxx and miny<=blob.cy()<=maxy):
            blue.on() # Turn on the blue LED to indicate that a mine is found
            to_wr="O{},{};".format(blob.cx(),blob.cy()) # Form the data to send
            print(to_wr)
            wr_str(to_wr) # Send data
    utime.sleep_ms(100) # Sleep for a while to ensure that data will not be send too fast to exceed the capacity of receiver
    blue.off() # Turn off the blue LED

    #print(clock.fps())
