This program sends a SIGNATURE command to the bootloader and then a page program command. It tests the reaction to the stk500v2 protocol and prints the returned values.

The serial port and leds should already be tested and working at this point by sending something in a loop and reading it with a terminal application to chek if it works.

The reference app note for the stk500 protocol is AVR068.pdf.
