EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ltc2630
LIBS:lt3010-5
LIBS:ad7280a
LIBS:myopto
LIBS:ina197
LIBS:hrs1k
LIBS:ftdichip
LIBS:ntd5867nl
LIBS:SPI_Flash
LIBS:si8422
LIBS:tja1052i
LIBS:nup2105l
LIBS:lt3990-3
LIBS:inductor-coilcraft
LIBS:bms-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 11
Title ""
Date "17 apr 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5150 3450 0    60   Input ~ 0
SDI
Text HLabel 8050 3450 2    60   Output ~ 0
SDO
Text HLabel 7100 3650 2    60   Input ~ 0
3V
Text HLabel 5150 3550 0    60   Input ~ 0
CLK
Text HLabel 5150 3750 0    60   Input ~ 0
CSFLASH/
Text HLabel 7100 3550 2    60   Input ~ 0
GND
$Comp
L M45PE16 U10
U 1 1 56CF3943
P 5950 3800
F 0 "U10" H 5800 3750 60  0000 C CNN
F 1 "M45PE16" H 5950 3650 60  0000 C CNN
F 2 "~" H 5950 3800 60  0000 C CNN
F 3 "~" H 5950 3800 60  0000 C CNN
	1    5950 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3450 5450 3450
Wire Wire Line
	8050 3450 7600 3450
Wire Wire Line
	5150 3550 5450 3550
Wire Wire Line
	5150 3750 5450 3750
Wire Wire Line
	7100 3650 6650 3650
Wire Wire Line
	6650 3750 6850 3750
Wire Wire Line
	6850 3750 6850 3650
Connection ~ 6850 3650
Wire Wire Line
	7100 3550 6650 3550
Text Label 6800 3650 0    60   ~ 0
3V
Text Label 6800 3550 0    60   ~ 0
GND
Wire Wire Line
	5450 3650 4250 3650
$Comp
L C C56
U 1 1 56CF3989
P 4350 4000
F 0 "C56" H 4350 4100 40  0000 L CNN
F 1 "0.1uF" H 4356 3915 40  0000 L CNN
F 2 "~" H 4388 3850 30  0000 C CNN
F 3 "~" H 4350 4000 60  0000 C CNN
	1    4350 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3800 4350 3650
Connection ~ 4350 3650
Wire Wire Line
	4350 4200 4350 4350
Wire Wire Line
	4350 4350 4500 4350
Text Label 4500 4350 0    60   ~ 0
GND
Text Label 4250 3650 0    60   ~ 0
3V
$Comp
L R R75
U 1 1 5714A559
P 7350 3450
F 0 "R75" V 7430 3450 40  0000 C CNN
F 1 "1K" V 7357 3451 40  0000 C CNN
F 2 "~" V 7280 3450 30  0000 C CNN
F 3 "~" H 7350 3450 30  0000 C CNN
	1    7350 3450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7100 3450 6650 3450
Text Label 6800 3450 0    60   ~ 0
FSDO
$EndSCHEMATC
