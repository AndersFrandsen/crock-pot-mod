#!/usr/bin/env python

import serial
import datetime
import time

logFile = "sous_vide_log_" + datetime.datetime.now().strftime("%Y-%m-%d-%H.%M.%S") + ".log"

with serial.Serial("/dev/ttyS0", baudrate = 19200) as ser, open(logFile, mode='wb') as logFile:
    try:
        while True:
            time.sleep(8)
            logFile.write(
                datetime.datetime.now().strftime("%Y-%m-%d-%H.%M.%S") + " - " + (ser.read(ser.inWaiting()))
            )
            logFile.flush()
    except KeyboardInterrupt:
        print("Stopped")
