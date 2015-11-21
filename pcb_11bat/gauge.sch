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
Sheet 6 9
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
L CONN_2 P21
U 1 1 560153A8
P 8450 3000
F 0 "P21" V 8400 3000 40  0000 C CNN
F 1 "CONN_2" V 8500 3000 40  0000 C CNN
F 2 "" H 8450 3000 60  0000 C CNN
F 3 "" H 8450 3000 60  0000 C CNN
	1    8450 3000
	1    0    0    -1  
$EndComp
Text HLabel 4950 2350 0    60   Input ~ 0
GND
Text HLabel 4950 1750 0    60   Input ~ 0
5V
Text HLabel 4950 2900 0    60   Input ~ 0
CSDAC/
Text HLabel 4950 3000 0    60   Input ~ 0
SCLK
Text HLabel 4950 3100 0    60   Input ~ 0
SDI
Wire Wire Line
	8100 2900 6750 2900
Wire Wire Line
	6750 3100 8100 3100
Wire Wire Line
	6750 3000 7050 3000
Wire Wire Line
	5600 2900 4950 2900
Wire Wire Line
	4950 3000 5600 3000
Wire Wire Line
	5600 3100 4950 3100
Wire Wire Line
	4950 2350 5450 2350
Wire Wire Line
	4950 1750 5450 1750
Text Label 5450 2350 0    60   ~ 0
GND
Text Label 5450 1750 0    60   ~ 0
5V
Text Label 7050 3000 0    60   ~ 0
5V
$Comp
L C C47
U 1 1 560154CA
P 5250 2050
F 0 "C47" H 5250 2150 40  0000 L CNN
F 1 "0.1uF" H 5256 1965 40  0000 L CNN
F 2 "~" H 5288 1900 30  0000 C CNN
F 3 "~" H 5250 2050 60  0000 C CNN
	1    5250 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2250 5250 2350
Connection ~ 5250 2350
Wire Wire Line
	5250 1850 5250 1750
Connection ~ 5250 1750
Text Label 7450 3100 0    60   ~ 0
GND
$Comp
L LTC2630 U6
U 1 1 56015491
P 6200 3200
F 0 "U6" H 6200 3150 60  0000 C CNN
F 1 "LTC2630" H 6200 3050 60  0000 C CNN
F 2 "~" H 6200 3150 60  0000 C CNN
F 3 "~" H 6200 3150 60  0000 C CNN
	1    6200 3200
	1    0    0    -1  
$EndComp
Text Label 7450 2900 0    60   ~ 0
GaugeValue
$EndSCHEMATC
