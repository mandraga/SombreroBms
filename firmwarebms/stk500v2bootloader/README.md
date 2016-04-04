Bootloader code.

Note that it works with avrdude 6.3 from 2016 because SPI_MULTI is disabled.

Fuse configuration for the atmega168pa:

LB: 0xD7 Crystal with BOD
HB: 0xDD 2.7V brown out detection level
EB: 0xFA Boot block uses the last 512Words 1024Bytes

Command line used to program them:

avrdude -c stk200 -p m168p -U lfuse:w:0xD7:m
avrdude -c stk200 -p m168p -U hfuse:w:0xDD:m
avrdude -c stk200 -p m168p -U efuse:w:0xFA:m

Command line to read the fuses:

avrdude -c stk200 -p m168p -U lfuse:r:dumpfile:h

Command line to read the lockbits:

avrdude -c stk200 -p m168p -U lock:r:dumpfile:h
