# Run on raspberry pi

#!/usr/bin/python

import spidev
import time

spi = spidev.SpiDev()
spi.open(0, 1)
spi.max_speed_hz = 1000000


i = 0
while True:
#    spi.xfer([0x55])
    spi.writebytes([i])
    print(spi.readbytes(1))
    time.sleep(0.5)
    i=i+1

