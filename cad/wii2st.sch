EESchema Schematic File Version 2  date 30/07/2013 21:46:36
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
LIBS:wii2st
LIBS:arduino_uno_r3_compatible-cache
LIBS:wii2st-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "30 jul 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PWR_FLAG #FLG01
U 1 1 51F817E6
P 6850 3150
F 0 "#FLG01" H 6850 3245 30  0001 C CNN
F 1 "PWR_FLAG" H 6850 3330 30  0000 C CNN
	1    6850 3150
	-1   0    0    1   
$EndComp
$Comp
L PWR_FLAG #FLG02
U 1 1 51F817DD
P 6650 3150
F 0 "#FLG02" H 6650 3245 30  0001 C CNN
F 1 "PWR_FLAG" H 6650 3330 30  0000 C CNN
	1    6650 3150
	-1   0    0    1   
$EndComp
Wire Wire Line
	7850 4500 7550 4500
Wire Wire Line
	7550 4500 7550 4100
Wire Wire Line
	7550 4100 6750 4100
Wire Wire Line
	6750 4600 7850 4600
Wire Wire Line
	3050 4500 2750 4500
Wire Wire Line
	3050 4800 1800 4800
Wire Wire Line
	1800 4800 1800 4700
Wire Wire Line
	7850 3900 7250 3900
Wire Wire Line
	7250 3900 7250 4000
Wire Wire Line
	7250 4000 7200 4000
Wire Wire Line
	7850 4400 7400 4400
Wire Wire Line
	7400 4400 7400 4500
Wire Wire Line
	7400 4500 7150 4500
Wire Wire Line
	6300 4000 6800 4000
Wire Wire Line
	6300 4500 6750 4500
Wire Wire Line
	6300 4400 6350 4400
Wire Wire Line
	4550 4800 4950 4800
Wire Wire Line
	4550 4300 4800 4300
Wire Wire Line
	4800 4300 4800 4700
Wire Wire Line
	4800 4700 4950 4700
Wire Wire Line
	4950 3600 2950 3600
Wire Wire Line
	2950 3600 2950 4500
Wire Wire Line
	6300 4100 6350 4100
Wire Wire Line
	6300 4600 6350 4600
Wire Wire Line
	6300 4300 6750 4300
Wire Wire Line
	6750 4400 7350 4400
Wire Wire Line
	7350 4400 7350 4200
Wire Wire Line
	7350 4200 7850 4200
Wire Wire Line
	2600 4700 2750 4700
Wire Wire Line
	2750 4700 2750 4500
Connection ~ 2950 4500
Wire Wire Line
	7150 4300 7300 4300
Wire Wire Line
	7300 4300 7300 4000
Wire Wire Line
	7300 4000 7850 4000
Wire Wire Line
	3050 4300 2700 4300
Wire Wire Line
	2700 4300 2700 4500
Wire Wire Line
	2700 4500 2600 4500
Text GLabel 6650 3150 1    60   Input ~ 0
Gnd
Text GLabel 6850 3150 1    60   Input ~ 0
Vcc
NoConn ~ 4950 3900
Text GLabel 7850 4100 0    60   Input ~ 0
Gnd
Text GLabel 1800 4500 0    60   Input ~ 0
Gnd
Text GLabel 3050 4600 0    60   Input ~ 0
Gnd
Text GLabel 4550 4500 2    60   Input ~ 0
Vcc
Text GLabel 4550 4600 2    60   Input ~ 0
Gnd
Text GLabel 4950 3800 0    60   Input ~ 0
Gnd
Text GLabel 4950 3700 0    60   Input ~ 0
Vcc
Text GLabel 7850 4300 0    60   Input ~ 0
Vcc
NoConn ~ 1800 4600
NoConn ~ 2600 4600
NoConn ~ 4550 4400
NoConn ~ 4550 4700
NoConn ~ 3050 4700
NoConn ~ 3050 4400
NoConn ~ 4950 3500
NoConn ~ 6300 4800
NoConn ~ 6300 4700
NoConn ~ 6300 4200
NoConn ~ 6300 3900
NoConn ~ 6300 3800
NoConn ~ 6300 3700
NoConn ~ 6300 3600
NoConn ~ 6300 3500
NoConn ~ 4950 4600
NoConn ~ 4950 4500
NoConn ~ 4950 4400
NoConn ~ 4950 4300
NoConn ~ 7850 3800
$Comp
L DIODE D1
U 1 1 51F81021
P 6550 4600
F 0 "D1" H 6550 4700 40  0000 C CNN
F 1 "DIODE" H 6550 4500 40  0000 C CNN
	1    6550 4600
	-1   0    0    -1  
$EndComp
$Comp
L DIODE D2
U 1 1 51F8101F
P 6950 4500
F 0 "D2" H 6950 4600 40  0000 C CNN
F 1 "DIODE" H 6950 4400 40  0000 C CNN
	1    6950 4500
	-1   0    0    -1  
$EndComp
$Comp
L DIODE D3
U 1 1 51F8101D
P 6550 4400
F 0 "D3" H 6550 4500 40  0000 C CNN
F 1 "DIODE" H 6550 4300 40  0000 C CNN
	1    6550 4400
	-1   0    0    -1  
$EndComp
$Comp
L DIODE D4
U 1 1 51F8101A
P 6950 4300
F 0 "D4" H 6950 4400 40  0000 C CNN
F 1 "DIODE" H 6950 4200 40  0000 C CNN
	1    6950 4300
	-1   0    0    -1  
$EndComp
$Comp
L DIODE D5
U 1 1 51F81015
P 6550 4100
F 0 "D5" H 6550 4200 40  0000 C CNN
F 1 "DIODE" H 6550 4000 40  0000 C CNN
	1    6550 4100
	-1   0    0    -1  
$EndComp
$Comp
L DIODE D6
U 1 1 51F8100A
P 7000 4000
F 0 "D6" H 7000 4100 40  0000 C CNN
F 1 "DIODE" H 7000 3900 40  0000 C CNN
	1    7000 4000
	-1   0    0    -1  
$EndComp
$Comp
L CONN_3X2 P1
U 1 1 51F80FA7
P 2200 4650
F 0 "P1" H 2200 4900 50  0000 C CNN
F 1 "CONN_3X2" V 2200 4700 40  0000 C CNN
	1    2200 4650
	1    0    0    -1  
$EndComp
$Comp
L DB9 J1
U 1 1 51F80F45
P 8300 4200
F 0 "J1" H 8300 4750 70  0000 C CNN
F 1 "DB9" H 8300 3650 70  0000 C CNN
	1    8300 4200
	1    0    0    -1  
$EndComp
$Comp
L LEVEL_CONVERTER LV1
U 1 1 51F80DED
P 3800 4150
F 0 "LV1" H 3800 4150 60  0000 C CNN
F 1 "LEVEL_CONVERTER" H 3800 4250 60  0000 C CNN
	1    3800 4150
	1    0    0    -1  
$EndComp
$Comp
L ARDUINO_DUEMILANOVE U1
U 1 1 51F80DD9
P 5650 4100
F 0 "U1" H 5650 4850 60  0000 C CNN
F 1 "ARDUINO_DUEMILANOVE" H 5650 4950 60  0000 C CNN
	1    5650 4100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
