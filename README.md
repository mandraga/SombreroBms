# SombreroBms

SombreroBMS is a battery management system board and software for small electric vehicles and conversions. Mainly 36V and 48V battery packs for golf carts.
Currently an unfinished project.

Planed features:

- Lithium phosphate battery packs.
- Protection relay to connect to a power relays like a Gigavac 250A.
- State of charge updated by a current sensor during the discharge.
- Custom gauge connected with one wire + Gnd serial communication.
- Monitoring from a portable computer using a serial communication over USB.
- Passive balancing (possibility for a heavy duty version for first balancing use).
- Undervoltage events count per battery + worst value recording.
- Powered from the battery pack.
- 10 to 18 batteries.

The development was interrupted when I moved to Lyon in 2016. And the client switched to Orion BMS, so the target was lost and the funding shorted.
However I started SombreroBMS2, simpler, easier to use and for lighter vehicles (however it will keep the same functionnality).
I will add more unit testing and simulation on it (hence the switch to gitlab).
On the second version the client is myself  :)

https://gitlab.com/arenyp/sombrerobms2

