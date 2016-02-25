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
LIBS:bms-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 10
Title ""
Date "25 feb 2016"
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
F 1 "1uF100V" H 7556 3565 40  0000 L CNN
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
Text HLabel 10150 3350 2    60   Input ~ 0
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
P 4650 4800
F 0 "C44" H 4650 4900 40  0000 L CNN
F 1 "0.1uF" H 4656 4715 40  0000 L CNN
F 2 "~" H 4688 4650 30  0000 C CNN
F 3 "~" H 4650 4800 60  0000 C CNN
	1    4650 4800
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
	6300 4000 9450 4000
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
$Comp
L EMI_FILTER2 FI?
U 1 1 56CE826B
P 9450 3350
F 0 "FI?" H 9450 3450 50  0000 C CNN
F 1 "EMI_FILTER2" H 9500 3200 50  0000 L CNN
F 2 "~" H 9450 3350 60  0000 C CNN
F 3 "~" H 9450 3350 60  0000 C CNN
	1    9450 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 4000 9450 3600
Connection ~ 8000 4000
Wire Wire Line
	9800 3350 10150 3350
$Comp
L LM117E U?
U 1 1 56CED544
P 3600 4600
F 0 "U?" H 3400 4800 40  0000 C CNN
F 1 "LM117E" H 3600 4800 40  0000 L CNN
F 2 "LLC-20" H 3600 4700 30  0000 C CIN
F 3 "" H 3600 4600 60  0000 C CNN
	1    3600 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 4550 3200 4550
Wire Wire Line
	3600 4850 3600 5050
Wire Wire Line
	4000 4550 5650 4550
Connection ~ 3600 5050
Text Label 2400 4550 0    60   ~ 0
5V
$Comp
L C C?
U 1 1 56CED685
P 4300 4800
F 0 "C?" H 4300 4900 40  0000 L CNN
F 1 "1uF" H 4306 4715 40  0000 L CNN
F 2 "~" H 4338 4650 30  0000 C CNN
F 3 "~" H 4300 4800 60  0000 C CNN
	1    4300 4800
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 56CED691
P 2850 4800
F 0 "C?" H 2850 4900 40  0000 L CNN
F 1 "1uF" H 2856 4715 40  0000 L CNN
F 2 "~" H 2888 4650 30  0000 C CNN
F 3 "~" H 2850 4800 60  0000 C CNN
	1    2850 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 4600 2850 4550
Connection ~ 2850 4550
Wire Wire Line
	2850 5000 2850 5050
Connection ~ 2850 5050
Wire Wire Line
	4300 4550 4300 4600
Connection ~ 4300 4550
Wire Wire Line
	4300 5000 4300 5050
Connection ~ 4300 5050
Wire Wire Line
	4650 4600 4650 4550
Connection ~ 4650 4550
Wire Wire Line
	4650 5000 4650 5050
Text Label 2400 5050 0    60   ~ 0
GND
Wire Wire Line
	4650 5050 2400 5050
Text Label 4950 4550 0    60   ~ 0
3V
Text HLabel 5650 4550 2    60   Output ~ 0
3V
$EndSCHEMATC
