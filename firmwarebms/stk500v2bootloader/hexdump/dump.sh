avrdude -p m168p -P /dev/ttyUSB0 -c stk500v2 -D -v -U flash:r:main.hex:h
