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
LIBS:ntd5867nl
LIBS:SPI_Flash
LIBS:si8422
LIBS:tja1052i
LIBS:nup2105l
LIBS:lt3990-3
LIBS:inductor-coilcraft
LIBS:tle4266
LIBS:ft232rq
LIBS:lm2936
LIBS:bms-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 11
Title ""
Date "2 may 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 4900 1050 1300 2250
U 55EA30DE
F0 "AD7280A_SUB_HI" 50
F1 "AD7280A_SUB.sch" 50
F2 "SCLK" I R 6200 1150 60 
F3 "SDI" I R 6200 1250 60 
F4 "SDO" O R 6200 1350 60 
F5 "ALERT" O R 6200 1550 60 
F6 "CS/" I R 6200 1450 60 
F7 "PD/" I R 6200 1650 60 
F8 "CNVST/" I R 6200 1750 60 
F9 "MASTER" I R 6200 2250 60 
F10 "SDO_IO" O R 6200 2350 60 
F11 "ALERT_IO" O R 6200 2450 60 
F12 "ALERThi" I L 4900 2850 60 
F13 "VSSN" I R 6200 2900 60 
F14 "VDDNFILTERED" O R 6200 2750 60 
F15 "LOBAT" I L 4900 1450 60 
F16 "SCLKhi" O L 4900 2550 60 
F17 "SDIhi" I L 4900 2650 60 
F18 "SDOhi" O L 4900 2750 60 
F19 "CShi/" O L 4900 2950 60 
F20 "PDhi/" O L 4900 3050 60 
F21 "CNVSThi/" O L 4900 3150 60 
F22 "HIBAT" O L 4900 1150 60 
F23 "VDRIVE" O R 6200 1900 60 
F24 "VREG" O R 6200 2000 60 
$EndSheet
$Comp
L R R21
U 1 1 55EA668F
P 7450 6050
F 0 "R21" V 7530 6050 40  0000 C CNN
F 1 "1K" V 7457 6051 40  0000 C CNN
F 2 "~" V 7380 6050 30  0000 C CNN
F 3 "~" H 7450 6050 30  0000 C CNN
	1    7450 6050
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 55EA6ABD
P 9250 3350
F 0 "C13" H 9250 3450 40  0000 L CNN
F 1 "22pF" H 9256 3265 40  0000 L CNN
F 2 "~" H 9288 3200 30  0000 C CNN
F 3 "~" H 9250 3350 60  0000 C CNN
	1    9250 3350
	0    -1   -1   0   
$EndComp
$Comp
L C C14
U 1 1 55EA6BBA
P 9250 3700
F 0 "C14" H 9250 3800 40  0000 L CNN
F 1 "22pF" H 9256 3615 40  0000 L CNN
F 2 "~" H 9288 3550 30  0000 C CNN
F 3 "~" H 9250 3700 60  0000 C CNN
	1    9250 3700
	0    -1   -1   0   
$EndComp
$Comp
L C C11
U 1 1 55EA6C32
P 9250 2700
F 0 "C11" H 9250 2800 40  0000 L CNN
F 1 "22pF" H 9256 2615 40  0000 L CNN
F 2 "~" H 9288 2550 30  0000 C CNN
F 3 "~" H 9250 2700 60  0000 C CNN
	1    9250 2700
	0    -1   -1   0   
$EndComp
$Comp
L C C12
U 1 1 55EA6C38
P 9250 3050
F 0 "C12" H 9250 3150 40  0000 L CNN
F 1 "22pF" H 9256 2965 40  0000 L CNN
F 2 "~" H 9288 2900 30  0000 C CNN
F 3 "~" H 9250 3050 60  0000 C CNN
	1    9250 3050
	0    -1   -1   0   
$EndComp
$Comp
L C C9
U 1 1 55EA6CB0
P 9250 2100
F 0 "C9" H 9250 2200 40  0000 L CNN
F 1 "22pF" H 9256 2015 40  0000 L CNN
F 2 "~" H 9288 1950 30  0000 C CNN
F 3 "~" H 9250 2100 60  0000 C CNN
	1    9250 2100
	0    -1   -1   0   
$EndComp
$Comp
L C C10
U 1 1 55EA6CB6
P 9250 2400
F 0 "C10" H 9250 2500 40  0000 L CNN
F 1 "22pF" H 9256 2315 40  0000 L CNN
F 2 "~" H 9288 2250 30  0000 C CNN
F 3 "~" H 9250 2400 60  0000 C CNN
	1    9250 2400
	0    -1   -1   0   
$EndComp
$Comp
L C C15
U 1 1 55EA6D2E
P 9250 4000
F 0 "C15" H 9250 4100 40  0000 L CNN
F 1 "22pF" H 9256 3915 40  0000 L CNN
F 2 "~" H 9288 3850 30  0000 C CNN
F 3 "~" H 9250 4000 60  0000 C CNN
	1    9250 4000
	0    1    1    0   
$EndComp
$Comp
L R R17
U 1 1 55EA6EFC
P 4350 2200
F 0 "R17" V 4430 2200 40  0000 C CNN
F 1 "1K" V 4357 2201 40  0000 C CNN
F 2 "~" V 4280 2200 30  0000 C CNN
F 3 "~" H 4350 2200 30  0000 C CNN
	1    4350 2200
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P8
U 1 1 55EAA821
P 2850 4650
F 0 "P8" H 2930 4650 40  0000 L CNN
F 1 "CONN_1" H 2850 4705 30  0001 C CNN
F 2 "" H 2850 4650 60  0000 C CNN
F 3 "" H 2850 4650 60  0000 C CNN
	1    2850 4650
	-1   0    0    1   
$EndComp
Text Label 3400 4650 2    60   ~ 0
VBAT-
$Comp
L R R24
U 1 1 55EABAD7
P 8200 2600
F 0 "R24" V 8280 2600 40  0000 C CNN
F 1 "1K" V 8207 2601 40  0000 C CNN
F 2 "~" V 8130 2600 30  0000 C CNN
F 3 "~" H 8200 2600 30  0000 C CNN
	1    8200 2600
	1    0    0    -1  
$EndComp
$Comp
L R R25
U 1 1 55EABB4F
P 8450 2600
F 0 "R25" V 8530 2600 40  0000 C CNN
F 1 "1K" V 8457 2601 40  0000 C CNN
F 2 "~" V 8380 2600 30  0000 C CNN
F 3 "~" H 8450 2600 30  0000 C CNN
	1    8450 2600
	1    0    0    -1  
$EndComp
$Comp
L R R23
U 1 1 55EABBC7
P 7950 2600
F 0 "R23" V 8030 2600 40  0000 C CNN
F 1 "10K" V 7957 2601 40  0000 C CNN
F 2 "~" V 7880 2600 30  0000 C CNN
F 3 "~" H 7950 2600 30  0000 C CNN
	1    7950 2600
	1    0    0    -1  
$EndComp
Text Label 1950 1450 2    60   ~ 0
MIDBAT
$Comp
L FILTER FB2
U 1 1 55EB19B6
P 3250 6250
F 0 "FB2" H 3250 6400 60  0000 C CNN
F 1 "FILTER" H 3250 6150 60  0000 C CNN
F 2 "~" H 3250 6250 60  0000 C CNN
F 3 "~" H 3250 6250 60  0000 C CNN
	1    3250 6250
	0    -1   -1   0   
$EndComp
Text Label 7050 2750 2    60   ~ 0
FILTEREDVBAT+
Text Label 2850 1850 0    60   ~ 0
FILTEREDVBAT+
Text Label 4500 5750 2    60   ~ 0
INTERSCLK
Text Label 4500 5850 2    60   ~ 0
INTERSDI
Text Label 6950 4350 2    60   ~ 0
SCLK
Text Label 6950 4450 2    60   ~ 0
SDI
Text Label 6950 4550 2    60   ~ 0
SDOOUT
Text Label 6950 4650 2    60   ~ 0
CS/
Text Label 6850 4750 2    60   ~ 0
ALERTOUT
Text Label 4500 6250 2    60   ~ 0
INTERPD/
Text Label 6950 4950 2    60   ~ 0
CNVST/
Text Label 4500 5950 2    60   ~ 0
INTERSDO
Text Label 4500 6150 2    60   ~ 0
INTERCS/
Text Label 4500 6050 2    60   ~ 0
INTERALERT
Text Label 6950 4850 2    60   ~ 0
PD/
Text Label 4500 6350 2    60   ~ 0
INTERCNVST/
Text Label 6950 1150 2    60   ~ 0
INTERSCLK
Text Label 6950 2350 2    60   ~ 0
INTERSDI
Text Label 6950 1650 2    60   ~ 0
INTERPD/
Text Label 6950 1250 2    60   ~ 0
INTERSDO
Text Label 6950 1450 2    60   ~ 0
INTERCS/
Text Label 6950 2450 2    60   ~ 0
INTERALERT
Text Label 6950 1750 2    60   ~ 0
INTERCNVST/
$Comp
L R R22
U 1 1 55EB87FA
P 7850 5900
F 0 "R22" V 7930 5900 40  0000 C CNN
F 1 "10K" V 7857 5901 40  0000 C CNN
F 2 "~" V 7780 5900 30  0000 C CNN
F 3 "~" H 7850 5900 30  0000 C CNN
	1    7850 5900
	1    0    0    -1  
$EndComp
Text Label 10050 2100 2    60   ~ 0
INTERSCLK
Text Label 10050 2400 2    60   ~ 0
INTERSDI
Text Label 10050 3700 2    60   ~ 0
INTERPD/
Text Label 10050 2700 2    60   ~ 0
INTERSDO
Text Label 10050 3050 2    60   ~ 0
INTERCS/
Text Label 10050 3350 2    60   ~ 0
INTERALERT
Text Label 10050 4000 2    60   ~ 0
INTERCNVST/
Text HLabel 8850 6750 2    60   Input ~ 0
GND
Text HLabel 7550 4350 2    60   Input ~ 0
SCLK
Text HLabel 7550 4450 2    60   Input ~ 0
SDI
Text HLabel 7550 4550 2    60   Output ~ 0
SDO
Text HLabel 7550 4650 2    60   Input ~ 0
CS/
Text HLabel 7550 4750 2    60   Output ~ 0
ALERT
Text HLabel 7550 4850 2    60   Input ~ 0
PD/
Text HLabel 7550 4950 2    60   Input ~ 0
CNVST/
$Comp
L R R15
U 1 1 5609E4FF
P 3850 2200
F 0 "R15" V 3930 2200 40  0000 C CNN
F 1 "1K" V 3857 2201 40  0000 C CNN
F 2 "~" V 3780 2200 30  0000 C CNN
F 3 "~" H 3850 2200 30  0000 C CNN
	1    3850 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 5550 7450 5550
Wire Wire Line
	7450 5550 7450 5800
Wire Wire Line
	6200 5650 7450 5650
Connection ~ 7450 5650
Wire Wire Line
	4900 4650 3000 4650
Wire Wire Line
	4900 4350 2350 4350
Wire Wire Line
	2350 4350 2350 1450
Wire Wire Line
	1950 1450 4900 1450
Wire Wire Line
	6200 1550 8200 1550
Wire Wire Line
	6200 1350 8450 1350
Connection ~ 2350 1450
Wire Wire Line
	2850 1850 4600 1850
Connection ~ 3600 1850
Wire Wire Line
	4900 5750 4500 5750
Wire Wire Line
	4900 5850 4500 5850
Wire Wire Line
	4900 5950 4500 5950
Wire Wire Line
	4900 6050 4500 6050
Wire Wire Line
	4900 6150 4500 6150
Wire Wire Line
	4900 6250 4500 6250
Wire Wire Line
	4900 6350 4500 6350
Wire Wire Line
	6600 6100 6600 6750
Wire Wire Line
	3250 6750 8850 6750
Wire Wire Line
	3250 6750 3250 6600
Wire Wire Line
	3250 5900 3250 4650
Connection ~ 3250 4650
Wire Wire Line
	6200 2900 8800 2900
Wire Wire Line
	6200 2750 7050 2750
Wire Wire Line
	6200 1150 6950 1150
Wire Wire Line
	6200 1250 6950 1250
Wire Wire Line
	6200 1450 6950 1450
Wire Wire Line
	6200 1650 6950 1650
Wire Wire Line
	6200 1750 6950 1750
Wire Wire Line
	6200 2250 7950 2250
Wire Wire Line
	7950 2250 7950 2350
Wire Wire Line
	8200 1550 8200 2350
Wire Wire Line
	8450 1350 8450 2350
Wire Wire Line
	8450 2900 8450 2850
Wire Wire Line
	7950 2850 7950 2900
Connection ~ 7950 2900
Wire Wire Line
	8200 2850 8200 2900
Connection ~ 8200 2900
Wire Wire Line
	6200 4350 7550 4350
Wire Wire Line
	6200 4450 7550 4450
Wire Wire Line
	6200 4550 7000 4550
Wire Wire Line
	6200 4650 7550 4650
Wire Wire Line
	6200 4750 7000 4750
Wire Wire Line
	6200 4850 7550 4850
Wire Wire Line
	6200 4950 7550 4950
Wire Wire Line
	7450 6300 7450 6500
Wire Wire Line
	7450 6500 6600 6500
Connection ~ 6600 6500
Wire Wire Line
	6200 5450 7850 5450
Wire Wire Line
	7850 5450 7850 5650
Wire Wire Line
	7850 6150 7850 6400
Wire Wire Line
	6850 6400 8800 6400
Wire Wire Line
	6850 5950 6850 6400
Wire Wire Line
	6200 2350 6950 2350
Wire Wire Line
	6200 2450 6950 2450
Wire Wire Line
	9050 2100 8800 2100
Wire Wire Line
	8800 2100 8800 6400
Wire Wire Line
	8800 4000 9050 4000
Connection ~ 8800 2900
Connection ~ 8450 2900
Connection ~ 7850 6400
Connection ~ 8800 4000
Wire Wire Line
	9050 2400 8800 2400
Connection ~ 8800 2400
Wire Wire Line
	9050 2700 8800 2700
Connection ~ 8800 2700
Wire Wire Line
	9050 3050 8800 3050
Connection ~ 8800 3050
Wire Wire Line
	9050 3350 8800 3350
Connection ~ 8800 3350
Wire Wire Line
	9050 3700 8800 3700
Connection ~ 8800 3700
Wire Wire Line
	9450 2100 10050 2100
Wire Wire Line
	9450 2400 10050 2400
Wire Wire Line
	9450 2700 10050 2700
Wire Wire Line
	9450 3050 10050 3050
Wire Wire Line
	9450 3350 10050 3350
Wire Wire Line
	9450 3700 10050 3700
Wire Wire Line
	9450 4000 10050 4000
Connection ~ 6600 6750
Wire Wire Line
	4900 2550 4600 2550
Wire Wire Line
	4600 2550 4600 2450
Wire Wire Line
	4600 1850 4600 1950
Wire Wire Line
	4350 1950 4350 1850
Connection ~ 4350 1850
Wire Wire Line
	4100 1850 4100 1950
Connection ~ 4100 1850
Wire Wire Line
	3850 1950 3850 1850
Connection ~ 3850 1850
Wire Wire Line
	3600 1950 3600 1850
Wire Wire Line
	3350 1850 3350 1950
Connection ~ 3350 1850
Connection ~ 3100 1850
Wire Wire Line
	4900 2650 4350 2650
Wire Wire Line
	4350 2650 4350 2450
Wire Wire Line
	4100 2450 4100 2750
Wire Wire Line
	4100 2750 4900 2750
Wire Wire Line
	4900 2850 3850 2850
Wire Wire Line
	3850 2850 3850 2450
Wire Wire Line
	3600 2450 3600 2950
Wire Wire Line
	3600 2950 4900 2950
Wire Wire Line
	4900 3050 3350 3050
Wire Wire Line
	3350 3050 3350 2450
Wire Wire Line
	3100 3150 4900 3150
Wire Wire Line
	3100 3150 3100 2450
Wire Wire Line
	3100 1950 3100 1850
$Comp
L R R19
U 1 1 5712D49C
P 7250 4550
F 0 "R19" V 7330 4550 40  0000 C CNN
F 1 "10K" V 7257 4551 40  0000 C CNN
F 2 "~" V 7180 4550 30  0000 C CNN
F 3 "~" H 7250 4550 30  0000 C CNN
	1    7250 4550
	0    -1   -1   0   
$EndComp
$Comp
L R R20
U 1 1 5712D621
P 7250 4750
F 0 "R20" V 7330 4750 40  0000 C CNN
F 1 "10K" V 7257 4751 40  0000 C CNN
F 2 "~" V 7180 4750 30  0000 C CNN
F 3 "~" H 7250 4750 30  0000 C CNN
	1    7250 4750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7500 4550 7550 4550
Wire Wire Line
	7500 4750 7550 4750
Wire Wire Line
	6200 1900 6350 1900
Wire Wire Line
	6350 1900 6350 2000
Wire Wire Line
	6350 2000 6200 2000
Wire Wire Line
	6200 5950 6850 5950
Wire Wire Line
	6600 6100 6200 6100
$Sheet
S 4900 4250 1300 2250
U 57134280
F0 "AD7280A_SUB_LO" 50
F1 "AD7280A_SUB.sch" 50
F2 "SCLK" I R 6200 4350 60 
F3 "SDI" I R 6200 4450 60 
F4 "SDO" O R 6200 4550 60 
F5 "ALERT" O R 6200 4750 60 
F6 "CS/" I R 6200 4650 60 
F7 "PD/" I R 6200 4850 60 
F8 "CNVST/" I R 6200 4950 60 
F9 "MASTER" I R 6200 5450 60 
F10 "SDO_IO" O R 6200 5550 60 
F11 "ALERT_IO" O R 6200 5650 60 
F12 "ALERThi" I L 4900 6050 60 
F13 "VSSN" I R 6200 6100 60 
F14 "VDDNFILTERED" O R 6200 5950 60 
F15 "LOBAT" I L 4900 4650 60 
F16 "SCLKhi" O L 4900 5750 60 
F17 "SDIhi" I L 4900 5850 60 
F18 "SDOhi" O L 4900 5950 60 
F19 "CShi/" O L 4900 6150 60 
F20 "PDhi/" O L 4900 6250 60 
F21 "CNVSThi/" O L 4900 6350 60 
F22 "HIBAT" O L 4900 4350 60 
F23 "VDRIVE" O R 6200 5100 60 
F24 "VREG" O R 6200 5200 60 
$EndSheet
Wire Wire Line
	6200 5100 8300 5100
Text HLabel 8300 5100 2    60   Input ~ 0
UCDRIVE
$Comp
L C C8
U 1 1 57135557
P 8150 5400
F 0 "C8" H 8150 5500 40  0000 L CNN
F 1 "0.1uF" H 8156 5315 40  0000 L CNN
F 2 "~" H 8188 5250 30  0000 C CNN
F 3 "~" H 8150 5400 60  0000 C CNN
	1    8150 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 5100 8150 5200
Connection ~ 8150 5100
Wire Wire Line
	8150 5600 8150 6750
Connection ~ 8150 6750
$Comp
L R R18
U 1 1 5609E3F7
P 4600 2200
F 0 "R18" V 4680 2200 40  0000 C CNN
F 1 "0R" V 4607 2201 40  0000 C CNN
F 2 "~" V 4530 2200 30  0000 C CNN
F 3 "~" H 4600 2200 30  0000 C CNN
	1    4600 2200
	1    0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 5609E47B
P 4100 2200
F 0 "R16" V 4180 2200 40  0000 C CNN
F 1 "0R" V 4107 2201 40  0000 C CNN
F 2 "~" V 4030 2200 30  0000 C CNN
F 3 "~" H 4100 2200 30  0000 C CNN
	1    4100 2200
	1    0    0    -1  
$EndComp
$Comp
L R R14
U 1 1 5609E2F5
P 3600 2200
F 0 "R14" V 3680 2200 40  0000 C CNN
F 1 "0R" V 3607 2201 40  0000 C CNN
F 2 "~" V 3530 2200 30  0000 C CNN
F 3 "~" H 3600 2200 30  0000 C CNN
	1    3600 2200
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 5609EEAC
P 3350 2200
F 0 "R13" V 3430 2200 40  0000 C CNN
F 1 "0R" V 3357 2201 40  0000 C CNN
F 2 "~" V 3280 2200 30  0000 C CNN
F 3 "~" H 3350 2200 30  0000 C CNN
	1    3350 2200
	1    0    0    -1  
$EndComp
$Comp
L R R12
U 1 1 568979D1
P 3100 2200
F 0 "R12" V 3180 2200 40  0000 C CNN
F 1 "0R" V 3107 2201 40  0000 C CNN
F 2 "~" V 3030 2200 30  0000 C CNN
F 3 "~" H 3100 2200 30  0000 C CNN
	1    3100 2200
	1    0    0    -1  
$EndComp
Text Label 7200 2900 0    60   ~ 0
VSS1
$EndSCHEMATC
