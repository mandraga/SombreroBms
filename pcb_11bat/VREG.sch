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
LIBS:max3311e
LIBS:ltc2630
LIBS:lt3010-5
LIBS:ad7280a
LIBS:myopto
LIBS:ina197
LIBS:hrs1k
LIBS:ftdichip
LIBS:bms-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 9
Title ""
Date "18 nov 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LT3010-5 U5
U 1 1 56016DFB
P 5600 4100
F 0 "U5" H 5600 4000 60  0000 C CNN
F 1 "LT3010-5" H 5650 3900 60  0000 C CNN
F 2 "~" H 5600 4100 60  0000 C CNN
F 3 "~" H 5600 4100 60  0000 C CNN
	1    5600 4100
	1    0    0    -1  
$EndComp
Text Label 6650 4000 0    60   ~ 0
GND
Text Label 4350 3350 0    60   ~ 0
5V
$Comp
L C C46
U 1 1 56016E96
P 7550 3650
F 0 "C46" H 7550 3750 40  0000 L CNN
F 1 "1uF50V" H 7556 3565 40  0000 L CNN
F 2 "~" H 7588 3500 30  0000 C CNN
F 3 "~" H 7550 3650 60  0000 C CNN
	1    7550 3650
	1    0    0    -1  
$EndComp
$Comp
L C C45
U 1 1 56016EB4
P 3800 3600
F 0 "C45" H 3800 3700 40  0000 L CNN
F 1 "1uF" H 3806 3515 40  0000 L CNN
F 2 "~" H 3838 3450 30  0000 C CNN
F 3 "~" H 3800 3600 60  0000 C CNN
	1    3800 3600
	1    0    0    -1  
$EndComp
Text Label 8650 3350 0    60   ~ 0
VBAT
Text HLabel 9100 3350 2    60   Input ~ 0
VBAT
Text Label 4050 3900 0    60   ~ 0
GND
Text HLabel 2400 3350 0    60   Output ~ 0
5V
Text HLabel 2400 3900 0    60   Input ~ 0
GND
$Comp
L C C44
U 1 1 56017B92
P 3100 3600
F 0 "C44" H 3100 3700 40  0000 L CNN
F 1 "0.1uF" H 3106 3515 40  0000 L CNN
F 2 "~" H 3138 3450 30  0000 C CNN
F 3 "~" H 3100 3600 60  0000 C CNN
	1    3100 3600
	1    0    0    -1  
$EndComp
$Comp
L ZENER D5
U 1 1 5601F9AB
P 8000 3650
F 0 "D5" H 8000 3750 50  0000 C CNN
F 1 "ZENER 64V" H 8000 3550 40  0000 C CNN
F 2 "~" H 8000 3650 60  0000 C CNN
F 3 "~" H 8000 3650 60  0000 C CNN
	1    8000 3650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2400 3350 4950 3350
Wire Wire Line
	6300 4000 8000 4000
Wire Wire Line
	4950 3650 4850 3650
Wire Wire Line
	4850 3650 4850 3900
Connection ~ 4850 3800
Wire Wire Line
	4950 3500 4850 3500
Wire Wire Line
	4850 3500 4850 3350
Connection ~ 4850 3350
Wire Wire Line
	7550 4000 7550 3850
Wire Wire Line
	7550 3450 7550 3350
Connection ~ 7550 3350
Wire Wire Line
	7150 3350 7150 3800
Wire Wire Line
	7150 3800 6300 3800
Connection ~ 7150 3350
Wire Wire Line
	3800 3400 3800 3350
Connection ~ 3800 3350
Wire Wire Line
	3800 3800 3800 3900
Wire Wire Line
	4850 3900 2400 3900
Connection ~ 3800 3900
Wire Wire Line
	3100 3350 3100 3400
Connection ~ 3100 3350
Wire Wire Line
	3100 3800 3100 3900
Connection ~ 3100 3900
Wire Wire Line
	6300 3350 9100 3350
Wire Wire Line
	8000 3350 8000 3450
Connection ~ 8000 3350
Wire Wire Line
	8000 4000 8000 3850
Connection ~ 7550 4000
Wire Wire Line
	6400 3500 6300 3500
Wire Wire Line
	6400 3650 6400 3500
Connection ~ 6400 3650
Text Label 6650 3650 0    60   ~ 0
GND
Wire Wire Line
	6300 3650 6650 3650
Wire Wire Line
	4850 3800 4950 3800
$EndSCHEMATC
