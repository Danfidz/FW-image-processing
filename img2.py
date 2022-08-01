#!/usr/bin/env python

from picamera import PiCamera
from time import sleep

camera = PiCamera()
camera.start_preview()
sleep(5)
camera.capture('/home/pi/Documents/img7.jpg')
camera.stop_preview()