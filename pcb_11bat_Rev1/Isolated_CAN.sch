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
Sheet 11 11
Title ""
Date "17 apr 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TJA1052I U12
U 1 1 571465EB
P 5050 5900
F 0 "U12" H 4850 5850 60  0000 C CNN
F 1 "TJA1052I" H 5000 5750 60  0000 C CNN
F 2 "~" H 5150 5900 60  0000 C CNN
F 3 "~" H 5150 5900 60  0000 C CNN
	1    5050 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 5150 4450 5150
Wire Wire Line
	2450 5850 4450 5850
Wire Wire Line
	4100 5250 4100 5850
Wire Wire Line
	4100 5750 4450 5750
Connection ~ 4100 5850
Wire Wire Line
	4450 5250 4100 5250
Wire Wire Line
	4450 5350 3650 5350
Wire Wire Line
	4450 5550 3650 5550
$Comp
L R R80
U 1 1 57146628
P 8650 5900
F 0 "R80" V 8730 5900 40  0000 C CNN
F 1 "4,7k" V 8657 5901 40  0000 C CNN
F 2 "~" V 8580 5900 30  0000 C CNN
F 3 "~" H 8650 5900 30  0000 C CNN
	1    8650 5900
	1    0    0    -1  
$EndComp
$Comp
L R R79
U 1 1 57146637
P 8650 5200
F 0 "R79" V 8730 5200 40  0000 C CNN
F 1 "4,7k" V 8657 5201 40  0000 C CNN
F 2 "~" V 8580 5200 30  0000 C CNN
F 3 "~" H 8650 5200 30  0000 C CNN
	1    8650 5200
	1    0    0    -1  
$EndComp
$Comp
L C C60
U 1 1 57146646
P 8350 5550
F 0 "C60" H 8350 5650 40  0000 L CNN
F 1 "4,7nF" H 8356 5465 40  0000 L CNN
F 2 "~" H 8388 5400 30  0000 C CNN
F 3 "~" H 8350 5550 60  0000 C CNN
	1    8350 5550
	0    -1   -1   0   
$EndComp
$Comp
L EMI_FILTER2 FI2
U 1 1 57146655
P 7750 3350
F 0 "FI2" H 7750 3450 50  0000 C CNN
F 1 "EMI_FILTER2" H 7800 3200 50  0000 L CNN
F 2 "~" H 7750 3350 60  0000 C CNN
F 3 "~" H 7750 3350 60  0000 C CNN
	1    7750 3350
	1    0    0    -1  
$EndComp
Text HLabel 2450 5850 0    60   Input ~ 0
BMSGND
Text HLabel 3650 5350 0    60   Input ~ 0
BMSCANTX
Text HLabel 3650 5550 0    60   Output ~ 0
BMSCANRX
Text HLabel 2450 5150 0    60   Input ~ 0
BMSVDD
Connection ~ 4100 5750
$Comp
L C C61
U 1 1 5714667A
P 8650 2100
F 0 "C61" H 8650 2200 40  0000 L CNN
F 1 "3uF" H 8656 2015 40  0000 L CNN
F 2 "~" H 8688 1950 30  0000 C CNN
F 3 "~" H 8650 2100 60  0000 C CNN
	1    8650 2100
	1    0    0    -1  
$EndComp
$Comp
L C C57
U 1 1 57146680
P 2750 5500
F 0 "C57" H 2750 5600 40  0000 L CNN
F 1 "0.1uF" H 2756 5415 40  0000 L CNN
F 2 "~" H 2788 5350 30  0000 C CNN
F 3 "~" H 2750 5500 60  0000 C CNN
	1    2750 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 3350 9500 3350
Wire Wire Line
	8050 1800 9150 1800
Wire Wire Line
	8650 1900 8650 1800
Connection ~ 8650 1800
Wire Wire Line
	8650 2300 8650 2450
Wire Wire Line
	7750 3600 7750 4000
Connection ~ 8650 2450
Wire Wire Line
	6400 5450 5750 5450
Wire Wire Line
	6400 4850 6400 5450
Wire Wire Line
	6400 5300 6700 5300
Wire Wire Line
	6400 4850 9400 4850
Wire Wire Line
	8650 4850 8650 4950
Connection ~ 6400 5300
Connection ~ 8650 4850
$Comp
L NUP2105L U13
U 1 1 57146BCE
P 7300 5850
F 0 "U13" H 7075 5790 60  0000 C CNN
F 1 "NUP2105L" H 7250 5705 60  0000 C CNN
F 2 "~" H 7300 5850 60  0000 C CNN
F 3 "~" H 7300 5850 60  0000 C CNN
	1    7300 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 5750 6400 5750
Wire Wire Line
	6400 5550 6400 6250
Wire Wire Line
	6400 5550 5750 5550
Wire Wire Line
	6400 6250 9500 6250
Connection ~ 6400 5750
Wire Wire Line
	8650 6250 8650 6150
Connection ~ 8650 6250
Wire Wire Line
	8650 5650 8650 5450
Wire Wire Line
	8550 5550 8650 5550
Connection ~ 8650 5550
Wire Wire Line
	8000 5550 8150 5550
Wire Wire Line
	8050 5550 8050 5900
Wire Wire Line
	8050 5900 8450 5900
Connection ~ 8050 5550
Text Label 9150 2450 2    60   ~ 0
CANGND
Text Label 8450 5900 2    60   ~ 0
CANGND
Wire Wire Line
	5750 5850 6300 5850
Wire Wire Line
	5900 5850 5900 5750
Wire Wire Line
	5900 5750 5750 5750
Connection ~ 5900 5850
Text Label 6300 5850 2    60   ~ 0
CANGND
Wire Wire Line
	5750 5250 6300 5250
Text Label 6300 5250 2    60   ~ 0
CANGND
Text Label 9150 1800 2    60   ~ 0
CANVDD
Text Label 6300 5150 2    60   ~ 0
CANVDD
Wire Wire Line
	6300 5150 5750 5150
Text Label 6300 5650 2    60   ~ 0
CANVDD
Wire Wire Line
	6300 5650 5750 5650
$Comp
L C C62
U 1 1 57146E2B
P 9000 5200
F 0 "C62" H 9000 5300 40  0000 L CNN
F 1 "60nF" H 9006 5115 40  0000 L CNN
F 2 "~" H 9038 5050 30  0000 C CNN
F 3 "~" H 9000 5200 60  0000 C CNN
	1    9000 5200
	1    0    0    -1  
$EndComp
$Comp
L C C63
U 1 1 57146E44
P 9000 5900
F 0 "C63" H 9000 6000 40  0000 L CNN
F 1 "60nF" H 9006 5815 40  0000 L CNN
F 2 "~" H 9038 5750 30  0000 C CNN
F 3 "~" H 9000 5900 60  0000 C CNN
	1    9000 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 5700 9000 5400
Wire Wire Line
	9000 5000 9000 4850
Connection ~ 9000 4850
Wire Wire Line
	9000 6100 9000 6250
Connection ~ 9000 6250
Wire Wire Line
	9000 5550 9450 5550
Connection ~ 9000 5550
Text Label 9450 5550 2    60   ~ 0
CANGND
Wire Wire Line
	8350 1900 8350 1800
Connection ~ 8350 1800
Connection ~ 8350 2450
Wire Wire Line
	2750 5300 2750 5150
Connection ~ 2750 5150
Wire Wire Line
	2750 5700 2750 5850
Connection ~ 2750 5850
$Comp
L CONN_4 P26
U 1 1 5714768F
P 10500 4500
F 0 "P26" V 10450 4500 50  0000 C CNN
F 1 "CONN_4" V 10550 4500 50  0000 C CNN
F 2 "" H 10500 4500 60  0000 C CNN
F 3 "" H 10500 4500 60  0000 C CNN
	1    10500 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 3350 9500 4350
Wire Wire Line
	9500 4350 10150 4350
Wire Wire Line
	9400 4850 9400 4450
Wire Wire Line
	9400 4450 10150 4450
Wire Wire Line
	9500 6250 9500 4550
Wire Wire Line
	9500 4550 10150 4550
Wire Wire Line
	10150 4650 9700 4650
Text Label 9700 4650 0    60   ~ 0
CANGND
Wire Wire Line
	5900 5350 5900 5250
Connection ~ 5900 5250
Wire Wire Line
	5900 5350 5750 5350
$Comp
L 7805 U14
U 1 1 57147A3D
P 7650 1850
F 0 "U14" H 7800 1654 60  0000 C CNN
F 1 "7805" H 7650 2050 60  0000 C CNN
F 2 "" H 7650 1850 60  0000 C CNN
F 3 "" H 7650 1850 60  0000 C CNN
	1    7650 1850
	1    0    0    -1  
$EndComp
$Comp
L C C58
U 1 1 57147A4A
P 7000 2100
F 0 "C58" H 7000 2200 40  0000 L CNN
F 1 "0.1uF60V" H 7006 2015 40  0000 L CNN
F 2 "~" H 7038 1950 30  0000 C CNN
F 3 "~" H 7000 2100 60  0000 C CNN
	1    7000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 1800 7000 1900
Wire Wire Line
	7000 2300 7000 2450
Connection ~ 7000 1800
Text Label 6450 4000 2    60   ~ 0
CANGND
Text Label 9500 3350 2    60   ~ 0
CANUNFILTERED12V
Text Label 6700 1800 2    60   ~ 0
CAN12V
Wire Wire Line
	6700 1800 7250 1800
Wire Wire Line
	7650 2100 7650 2450
Wire Wire Line
	7000 2450 9150 2450
Connection ~ 7650 2450
$Comp
L DIODE D11
U 1 1 57147D86
P 7050 3350
F 0 "D11" H 7050 3450 40  0000 C CNN
F 1 "DIODE" H 7050 3250 40  0000 C CNN
F 2 "~" H 7050 3350 60  0000 C CNN
F 3 "~" H 7050 3350 60  0000 C CNN
	1    7050 3350
	-1   0    0    1   
$EndComp
Wire Wire Line
	6450 3350 6850 3350
Wire Wire Line
	7250 3350 7400 3350
Wire Wire Line
	7750 4000 6450 4000
Text Label 6400 4850 0    60   ~ 0
CANH
Text Label 6400 6250 0    60   ~ 0
CANL
Text Label 6450 3350 2    60   ~ 0
CAN12V
$Comp
L C C59
U 1 1 57146FEB
P 8350 2100
F 0 "C59" H 8350 2200 40  0000 L CNN
F 1 "0.1uF" H 8356 2015 40  0000 L CNN
F 2 "~" H 8388 1950 30  0000 C CNN
F 3 "~" H 8350 2100 60  0000 C CNN
	1    8350 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 2300 8350 2450
$EndSCHEMATC
