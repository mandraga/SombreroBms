testing a simple stk500 command from the computer to check if the bootloader works.
When porting the boot loader by Peter Fleury and others, I noticed how the version I picked up
did not work out of the box. Once for the Atmega128 and once for the Atmega168p.
And because I will use other Atmel chips like the atmega32C1, it is nice to have a test app to check if a stk500 command passes.
The reference app note for the stk500 protocol is AVR068.pdf.

The serial port and leds should already be tested and working at this point.
This only tests the protocol.
It was avrdude version...
