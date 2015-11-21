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
LIBS:ftdichip
LIBS:max3311e
LIBS:ltc2630
LIBS:lt3010-5
LIBS:ad7280a
LIBS:myopto
LIBS:ina197
LIBS:hrs1k
LIBS:bms-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 9
Title ""
Date "6 oct 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4500 3550 0    60   Input ~ 0
TX
Text HLabel 4500 3650 0    60   Output ~ 0
RX
Text HLabel 3400 3250 0    60   Input ~ 0
5V
Text HLabel 4500 3450 0    60   Input ~ 0
SHDN/
Text HLabel 2800 3950 0    60   Input ~ 0
GND
$Comp
L MAX3311E U5
U 1 1 560032C8
P 5850 3700
F 0 "U5" H 5800 3600 60  0000 C CNN
F 1 "MAX3311E" H 5850 3500 60  0000 C CNN
F 2 "~" H 5850 3500 60  0000 C CNN
F 3 "~" H 5850 3500 60  0000 C CNN
	1    5850 3700
	1    0    0    -1  
$EndComp
$Comp
L CP1 C43
U 1 1 560032F5
P 5850 2800
F 0 "C43" H 5900 2900 50  0000 L CNN
F 1 "1uF" H 5900 2700 50  0000 L CNN
F 2 "~" H 5850 2800 60  0000 C CNN
F 3 "~" H 5850 2800 60  0000 C CNN
	1    5850 2800
	0    1    1    0   
$EndComp
$Comp
L CP1 C42
U 1 1 5600330E
P 3800 3600
F 0 "C42" H 3850 3700 50  0000 L CNN
F 1 "1uF" H 3850 3500 50  0000 L CNN
F 2 "~" H 3800 3600 60  0000 C CNN
F 3 "~" H 3800 3600 60  0000 C CNN
	1    3800 3600
	1    0    0    -1  
$EndComp
$Comp
L CP1 C44
U 1 1 56003314
P 7650 3650
F 0 "C44" H 7700 3750 50  0000 L CNN
F 1 "1uF" H 7700 3550 50  0000 L CNN
F 2 "~" H 7650 3650 60  0000 C CNN
F 3 "~" H 7650 3650 60  0000 C CNN
	1    7650 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	6600 3350 7200 3350
Wire Wire Line
	7200 3350 7200 2800
Wire Wire Line
	7200 2800 6050 2800
Wire Wire Line
	5050 3350 4500 3350
Wire Wire Line
	4500 3350 4500 2800
Wire Wire Line
	4500 2800 5650 2800
Wire Wire Line
	3400 3250 5050 3250
Wire Wire Line
	3800 3250 3800 3400
Wire Wire Line
	6600 3250 6950 3250
Wire Wire Line
	3800 3800 3800 3950
Wire Wire Line
	3800 3950 2800 3950
Text Label 3350 3950 0    60   ~ 0
GND
Text Label 6950 3250 0    60   ~ 0
GND
Wire Wire Line
	6600 3450 7650 3450
Wire Wire Line
	7650 3850 7650 3950
Wire Wire Line
	7000 3950 7900 3950
Text Label 7900 3950 0    60   ~ 0
GND
Wire Wire Line
	5050 3450 4500 3450
Connection ~ 3800 3250
$Comp
L DB9 J1
U 1 1 560033C6
P 7450 4600
F 0 "J1" H 7450 5150 70  0000 C CNN
F 1 "DB9" H 7450 4050 70  0000 C CNN
F 2 "" H 7450 4600 60  0000 C CNN
F 3 "" H 7450 4600 60  0000 C CNN
	1    7450 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3550 4500 3550
Wire Wire Line
	4500 3650 5050 3650
Wire Wire Line
	6600 3550 6800 3550
Wire Wire Line
	6800 3550 6800 4800
Wire Wire Line
	6800 4800 7000 4800
Wire Wire Line
	7000 4600 6900 4600
Wire Wire Line
	6900 4600 6900 3650
Wire Wire Line
	6900 3650 6600 3650
Wire Wire Line
	7000 3950 7000 4200
Connection ~ 7650 3950
NoConn ~ 7000 5000
NoConn ~ 7000 4900
NoConn ~ 7000 4700
NoConn ~ 7000 4500
NoConn ~ 7000 4300
NoConn ~ 7000 4400
$EndSCHEMATC
