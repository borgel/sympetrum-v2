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
LIBS:Borgel Custom
LIBS:sympetrum-v2 FF1.1-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L +3V3 #PWR203
U 1 1 589AA607
P 4850 2000
F 0 "#PWR203" H 4850 1850 50  0001 C CNN
F 1 "+3V3" H 4850 2140 50  0000 C CNN
F 2 "" H 4850 2000 50  0000 C CNN
F 3 "" H 4850 2000 50  0000 C CNN
	1    4850 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR204
U 1 1 589AA82F
P 4850 3650
F 0 "#PWR204" H 4850 3400 50  0001 C CNN
F 1 "GND" H 4850 3500 50  0000 C CNN
F 2 "" H 4850 3650 50  0000 C CNN
F 3 "" H 4850 3650 50  0000 C CNN
	1    4850 3650
	1    0    0    -1  
$EndComp
$Comp
L C_Small C202
U 1 1 58A60A76
P 7700 1450
F 0 "C202" H 7710 1520 50  0000 L CNN
F 1 "22uF" H 7710 1370 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7700 1450 50  0001 C CNN
F 3 "" H 7700 1450 50  0001 C CNN
F 4 "Samsung" H 0   0   50  0001 C CNN "MFR"
F 5 "CL21A226KQCLRNC" H 0   0   50  0001 C CNN "MPN"
F 6 "DigiKey" H 0   0   50  0001 C CNN "SPR"
F 7 "1276-6687-1-ND" H 0   0   50  0001 C CNN "SPN"
F 8 "" H 0   0   50  0001 C CNN "SPURL"
	1    7700 1450
	1    0    0    -1  
$EndComp
$Comp
L C_Small C203
U 1 1 58A60B1A
P 8050 1450
F 0 "C203" H 8060 1520 50  0000 L CNN
F 1 "22uF" H 8060 1370 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 8050 1450 50  0001 C CNN
F 3 "" H 8050 1450 50  0001 C CNN
F 4 "Samsung" H 0   0   50  0001 C CNN "MFR"
F 5 "CL21A226KQCLRNC" H 0   0   50  0001 C CNN "MPN"
F 6 "DigiKey" H 0   0   50  0001 C CNN "SPR"
F 7 "1276-6687-1-ND" H 0   0   50  0001 C CNN "SPN"
F 8 "" H 0   0   50  0001 C CNN "SPURL"
	1    8050 1450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR206
U 1 1 58A60B53
P 7900 1700
F 0 "#PWR206" H 7900 1450 50  0001 C CNN
F 1 "GND" H 7900 1550 50  0000 C CNN
F 2 "" H 7900 1700 50  0000 C CNN
F 3 "" H 7900 1700 50  0000 C CNN
	1    7900 1700
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR205
U 1 1 58A60B8B
P 7900 1150
F 0 "#PWR205" H 7900 1000 50  0001 C CNN
F 1 "+3V3" H 7900 1290 50  0000 C CNN
F 2 "" H 7900 1150 50  0000 C CNN
F 3 "" H 7900 1150 50  0000 C CNN
	1    7900 1150
	1    0    0    -1  
$EndComp
$Comp
L C_Small C201
U 1 1 58A61C9E
P 7400 1450
F 0 "C201" H 7410 1520 50  0000 L CNN
F 1 "22uF" H 7410 1370 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7400 1450 50  0001 C CNN
F 3 "" H 7400 1450 50  0001 C CNN
F 4 "Samsung" H 0   0   50  0001 C CNN "MFR"
F 5 "CL21A226KQCLRNC" H 0   0   50  0001 C CNN "MPN"
F 6 "DigiKey" H 0   0   50  0001 C CNN "SPR"
F 7 "1276-6687-1-ND" H 0   0   50  0001 C CNN "SPN"
F 8 "" H 0   0   50  0001 C CNN "SPURL"
	1    7400 1450
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U201
U 1 1 58C5DB20
P 2650 2700
F 0 "U201" H 2450 3000 60  0000 C CNN
F 1 "APA102_5050" H 2650 2400 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 2650 2300 60  0001 C CNN
F 3 "" H 2650 2250 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 2650 3250 60  0001 C CNN "Notes"
F 5 "" H 0   0   50  0001 C CNN "MFR"
F 6 "" H 0   0   50  0001 C CNN "MPN"
F 7 "" H 0   0   50  0001 C CNN "SPR"
F 8 "" H 0   0   50  0001 C CNN "SPN"
F 9 "" H 0   0   50  0001 C CNN "SPURL"
	1    2650 2700
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U202
U 1 1 58C5DC9A
P 3900 2700
F 0 "U202" H 3700 3000 60  0000 C CNN
F 1 "APA102_5050" H 3900 2400 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 3900 2300 60  0001 C CNN
F 3 "" H 3900 2250 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 3900 3250 60  0001 C CNN "Notes"
F 5 "" H 150 0   50  0001 C CNN "MFR"
F 6 "" H 150 0   50  0001 C CNN "MPN"
F 7 "" H 150 0   50  0001 C CNN "SPR"
F 8 "" H 150 0   50  0001 C CNN "SPN"
F 9 "" H 150 0   50  0001 C CNN "SPURL"
	1    3900 2700
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U203
U 1 1 58C5DCC8
P 5200 2700
F 0 "U203" H 5000 3000 60  0000 C CNN
F 1 "APA102_5050" H 5200 2400 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 5200 2300 60  0001 C CNN
F 3 "" H 5200 2250 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 5200 3250 60  0001 C CNN "Notes"
F 5 "" H 350 0   50  0001 C CNN "MFR"
F 6 "" H 350 0   50  0001 C CNN "MPN"
F 7 "" H 350 0   50  0001 C CNN "SPR"
F 8 "" H 350 0   50  0001 C CNN "SPN"
F 9 "" H 350 0   50  0001 C CNN "SPURL"
	1    5200 2700
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U204
U 1 1 58C5DCFA
P 6450 2700
F 0 "U204" H 6250 3000 60  0000 C CNN
F 1 "APA102_5050" H 6450 2400 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 6450 2300 60  0001 C CNN
F 3 "" H 6450 2250 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 6450 3250 60  0001 C CNN "Notes"
F 5 "" H 500 0   50  0001 C CNN "MFR"
F 6 "" H 500 0   50  0001 C CNN "MPN"
F 7 "" H 500 0   50  0001 C CNN "SPR"
F 8 "" H 500 0   50  0001 C CNN "SPN"
F 9 "" H 500 0   50  0001 C CNN "SPURL"
	1    6450 2700
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U205
U 1 1 58C5DD2C
P 7800 2700
F 0 "U205" H 7600 3000 60  0000 C CNN
F 1 "APA102_5050" H 7800 2400 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 7800 2300 60  0001 C CNN
F 3 "" H 7800 2250 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 7800 3250 60  0001 C CNN "Notes"
F 5 "" H 750 0   50  0001 C CNN "MFR"
F 6 "" H 750 0   50  0001 C CNN "MPN"
F 7 "" H 750 0   50  0001 C CNN "SPR"
F 8 "" H 750 0   50  0001 C CNN "SPN"
F 9 "" H 750 0   50  0001 C CNN "SPURL"
	1    7800 2700
	1    0    0    -1  
$EndComp
Text HLabel 1850 2850 0    60   Input ~ 0
MOSI-IN
Text HLabel 1850 2700 0    60   Input ~ 0
SCK-IN
Text HLabel 8800 5400 2    60   Output ~ 0
SCK-OUT
Text HLabel 8800 5550 2    60   Output ~ 0
MOSI-OUT
Text Notes 8250 1150 0    60   ~ 0
Bulk capacitance, place\non the long parts of the rails
$Comp
L Jumper_NO_Small JP201
U 1 1 590F882C
P 2650 3150
F 0 "JP201" H 2650 3230 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2660 3090 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 2650 3150 50  0001 C CNN
F 3 "" H 2650 3150 50  0001 C CNN
	1    2650 3150
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP202
U 1 1 590F891B
P 2650 3350
F 0 "JP202" H 2650 3430 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2660 3290 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 2650 3350 50  0001 C CNN
F 3 "" H 2650 3350 50  0001 C CNN
	1    2650 3350
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP207
U 1 1 590F961A
P 3900 3150
F 0 "JP207" H 3900 3230 50  0000 C CNN
F 1 "Jumper_NO_Small" H 3910 3090 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 3900 3150 50  0001 C CNN
F 3 "" H 3900 3150 50  0001 C CNN
	1    3900 3150
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP208
U 1 1 590F96B8
P 3900 3350
F 0 "JP208" H 3900 3430 50  0000 C CNN
F 1 "Jumper_NO_Small" H 3910 3290 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 3900 3350 50  0001 C CNN
F 3 "" H 3900 3350 50  0001 C CNN
	1    3900 3350
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP211
U 1 1 590F974D
P 5200 3150
F 0 "JP211" H 5200 3230 50  0000 C CNN
F 1 "Jumper_NO_Small" H 5210 3090 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 5200 3150 50  0001 C CNN
F 3 "" H 5200 3150 50  0001 C CNN
	1    5200 3150
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP212
U 1 1 590F9753
P 5200 3350
F 0 "JP212" H 5200 3430 50  0000 C CNN
F 1 "Jumper_NO_Small" H 5210 3290 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 5200 3350 50  0001 C CNN
F 3 "" H 5200 3350 50  0001 C CNN
	1    5200 3350
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP215
U 1 1 590F9859
P 6450 3150
F 0 "JP215" H 6450 3230 50  0000 C CNN
F 1 "Jumper_NO_Small" H 6460 3090 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 6450 3150 50  0001 C CNN
F 3 "" H 6450 3150 50  0001 C CNN
	1    6450 3150
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP216
U 1 1 590F985F
P 6450 3350
F 0 "JP216" H 6450 3430 50  0000 C CNN
F 1 "Jumper_NO_Small" H 6460 3290 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 6450 3350 50  0001 C CNN
F 3 "" H 6450 3350 50  0001 C CNN
	1    6450 3350
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP219
U 1 1 590F98C5
P 7800 3150
F 0 "JP219" H 7800 3230 50  0000 C CNN
F 1 "Jumper_NO_Small" H 7810 3090 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 7800 3150 50  0001 C CNN
F 3 "" H 7800 3150 50  0001 C CNN
	1    7800 3150
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP220
U 1 1 590F98CB
P 7800 3350
F 0 "JP220" H 7800 3430 50  0000 C CNN
F 1 "Jumper_NO_Small" H 7810 3290 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 7800 3350 50  0001 C CNN
F 3 "" H 7800 3350 50  0001 C CNN
	1    7800 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 2700 2200 2700
Wire Wire Line
	1850 2850 2200 2850
Wire Wire Line
	6900 2700 7350 2700
Wire Wire Line
	6900 2850 7350 2850
Wire Wire Line
	5650 2700 6000 2700
Wire Wire Line
	5650 2850 6000 2850
Wire Wire Line
	4350 2700 4750 2700
Wire Wire Line
	4350 2850 4750 2850
Wire Wire Line
	3100 2700 3450 2700
Wire Wire Line
	3100 2850 3450 2850
Wire Wire Line
	1800 4050 1800 5400
Wire Wire Line
	8600 2700 8600 4050
Wire Wire Line
	8500 2850 8500 3950
Wire Wire Line
	1700 3950 1700 5550
Wire Wire Line
	7900 1150 7900 1250
Wire Wire Line
	7700 1250 7700 1350
Wire Wire Line
	7700 1550 7700 1650
Wire Wire Line
	7900 1650 7900 1700
Wire Wire Line
	8050 1650 8050 1550
Wire Wire Line
	8050 1250 8050 1350
Wire Wire Line
	7400 1250 7400 1350
Wire Wire Line
	7400 1550 7400 1650
Wire Wire Line
	8350 2550 8250 2550
Wire Wire Line
	3150 3650 3150 2550
Wire Wire Line
	3150 2550 3100 2550
Wire Wire Line
	4850 2000 4850 2150
Wire Wire Line
	2100 2150 7350 2150
Wire Wire Line
	2100 2150 2100 2550
Wire Wire Line
	2100 2550 2200 2550
Wire Wire Line
	6000 2550 5950 2550
Wire Wire Line
	4350 2550 4550 2550
Wire Wire Line
	5650 2550 5850 2550
Wire Wire Line
	6900 2550 7150 2550
Wire Wire Line
	7400 1650 8050 1650
Wire Wire Line
	7400 1250 8050 1250
Connection ~ 7900 1650
Connection ~ 7900 1250
Connection ~ 7700 1250
Connection ~ 7700 1650
Connection ~ 8050 1250
Connection ~ 8050 1650
Connection ~ 4850 3650
Connection ~ 4850 2150
Wire Wire Line
	8600 4050 1800 4050
Wire Wire Line
	8500 3950 1700 3950
Wire Wire Line
	2150 2700 2150 3150
Wire Wire Line
	2150 3150 2550 3150
Connection ~ 2150 2700
Wire Wire Line
	2750 3150 3200 3150
Wire Wire Line
	2550 3350 2100 3350
Wire Wire Line
	2100 3350 2100 2850
Connection ~ 2100 2850
Wire Wire Line
	2750 3350 3250 3350
Wire Wire Line
	8250 2850 8500 2850
Wire Wire Line
	8250 2700 8600 2700
Wire Wire Line
	7350 2150 7350 2550
Wire Wire Line
	8350 3650 8350 2550
Wire Wire Line
	3150 3650 8350 3650
Wire Wire Line
	7150 2550 7150 3650
Connection ~ 7150 3650
Wire Wire Line
	7700 3150 7300 3150
Wire Wire Line
	7300 3150 7300 2700
Connection ~ 7300 2700
Wire Wire Line
	7250 2850 7250 3350
Wire Wire Line
	7250 3350 7700 3350
Connection ~ 7250 2850
Wire Wire Line
	7900 3150 8300 3150
Wire Wire Line
	8300 3150 8300 2700
Connection ~ 8300 2700
Wire Wire Line
	8400 3350 7900 3350
Wire Wire Line
	6550 3150 6950 3150
Wire Wire Line
	6950 3150 6950 2700
Connection ~ 6950 2700
Wire Wire Line
	6550 3350 7000 3350
Wire Wire Line
	7000 3350 7000 2850
Connection ~ 7000 2850
Wire Wire Line
	3450 2550 3400 2550
Wire Wire Line
	3400 2550 3400 2150
Connection ~ 3400 2150
Wire Wire Line
	3200 3150 3200 2700
Connection ~ 3200 2700
Wire Wire Line
	3250 3350 3250 2850
Connection ~ 3250 2850
Wire Wire Line
	3800 3350 3350 3350
Wire Wire Line
	3350 3350 3350 2850
Connection ~ 3350 2850
Wire Wire Line
	3800 3150 3400 3150
Wire Wire Line
	3400 3150 3400 2700
Connection ~ 3400 2700
Wire Wire Line
	4750 2550 4650 2550
Wire Wire Line
	4650 2550 4650 2150
Connection ~ 4650 2150
Wire Wire Line
	4550 2550 4550 3650
Connection ~ 4550 3650
Wire Wire Line
	5850 2550 5850 3650
Connection ~ 5850 3650
Wire Wire Line
	5950 2550 5950 2150
Connection ~ 5950 2150
Wire Wire Line
	5100 3150 4700 3150
Wire Wire Line
	4700 3150 4700 2700
Connection ~ 4700 2700
Wire Wire Line
	4650 2850 4650 3350
Wire Wire Line
	4650 3350 5100 3350
Connection ~ 4650 2850
Wire Wire Line
	5300 3150 5700 3150
Wire Wire Line
	5700 3150 5700 2700
Connection ~ 5700 2700
Wire Wire Line
	5300 3350 5750 3350
Wire Wire Line
	5750 3350 5750 2850
Connection ~ 5750 2850
Wire Wire Line
	6350 3150 5950 3150
Wire Wire Line
	5950 3150 5950 2700
Connection ~ 5950 2700
Wire Wire Line
	5900 2850 5900 3350
Wire Wire Line
	5900 3350 6350 3350
Connection ~ 5900 2850
Wire Wire Line
	4000 3150 4400 3150
Wire Wire Line
	4400 3150 4400 2700
Connection ~ 4400 2700
Wire Wire Line
	4000 3350 4450 3350
Wire Wire Line
	4450 3350 4450 2850
Connection ~ 4450 2850
Wire Wire Line
	8400 3350 8400 2850
Connection ~ 8400 2850
$Comp
L +3V3 #PWR201
U 1 1 590FCE05
P 4800 4700
F 0 "#PWR201" H 4800 4550 50  0001 C CNN
F 1 "+3V3" H 4800 4840 50  0000 C CNN
F 2 "" H 4800 4700 50  0000 C CNN
F 3 "" H 4800 4700 50  0000 C CNN
	1    4800 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR202
U 1 1 590FCE0B
P 4800 6350
F 0 "#PWR202" H 4800 6100 50  0001 C CNN
F 1 "GND" H 4800 6200 50  0000 C CNN
F 2 "" H 4800 6350 50  0000 C CNN
F 3 "" H 4800 6350 50  0000 C CNN
	1    4800 6350
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U206
U 1 1 590FCE1D
P 2600 5400
F 0 "U206" H 2400 5700 60  0000 C CNN
F 1 "APA102_5050" H 2600 5100 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 2600 5000 60  0001 C CNN
F 3 "" H 2600 4950 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 2600 5950 60  0001 C CNN "Notes"
F 5 "" H -50 2700 50  0001 C CNN "MFR"
F 6 "" H -50 2700 50  0001 C CNN "MPN"
F 7 "" H -50 2700 50  0001 C CNN "SPR"
F 8 "" H -50 2700 50  0001 C CNN "SPN"
F 9 "" H -50 2700 50  0001 C CNN "SPURL"
	1    2600 5400
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U207
U 1 1 590FCE29
P 3850 5400
F 0 "U207" H 3650 5700 60  0000 C CNN
F 1 "APA102_5050" H 3850 5100 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 3850 5000 60  0001 C CNN
F 3 "" H 3850 4950 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 3850 5950 60  0001 C CNN "Notes"
F 5 "" H 100 2700 50  0001 C CNN "MFR"
F 6 "" H 100 2700 50  0001 C CNN "MPN"
F 7 "" H 100 2700 50  0001 C CNN "SPR"
F 8 "" H 100 2700 50  0001 C CNN "SPN"
F 9 "" H 100 2700 50  0001 C CNN "SPURL"
	1    3850 5400
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U208
U 1 1 590FCE35
P 5150 5400
F 0 "U208" H 4950 5700 60  0000 C CNN
F 1 "APA102_5050" H 5150 5100 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 5150 5000 60  0001 C CNN
F 3 "" H 5150 4950 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 5150 5950 60  0001 C CNN "Notes"
F 5 "" H 300 2700 50  0001 C CNN "MFR"
F 6 "" H 300 2700 50  0001 C CNN "MPN"
F 7 "" H 300 2700 50  0001 C CNN "SPR"
F 8 "" H 300 2700 50  0001 C CNN "SPN"
F 9 "" H 300 2700 50  0001 C CNN "SPURL"
	1    5150 5400
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U209
U 1 1 590FCE41
P 6400 5400
F 0 "U209" H 6200 5700 60  0000 C CNN
F 1 "APA102_5050" H 6400 5100 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 6400 5000 60  0001 C CNN
F 3 "" H 6400 4950 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 6400 5950 60  0001 C CNN "Notes"
F 5 "" H 450 2700 50  0001 C CNN "MFR"
F 6 "" H 450 2700 50  0001 C CNN "MPN"
F 7 "" H 450 2700 50  0001 C CNN "SPR"
F 8 "" H 450 2700 50  0001 C CNN "SPN"
F 9 "" H 450 2700 50  0001 C CNN "SPURL"
	1    6400 5400
	1    0    0    -1  
$EndComp
$Comp
L APA102_5050 U210
U 1 1 590FCE4D
P 7750 5400
F 0 "U210" H 7550 5700 60  0000 C CNN
F 1 "APA102_5050" H 7750 5100 60  0000 C CNN
F 2 "Borgel Custom:LED-5050" H 7750 5000 60  0001 C CNN
F 3 "" H 7750 4950 60  0001 C CNN
F 4 "Choose one or both GND and VDD" H 7750 5950 60  0001 C CNN "Notes"
F 5 "" H 700 2700 50  0001 C CNN "MFR"
F 6 "" H 700 2700 50  0001 C CNN "MPN"
F 7 "" H 700 2700 50  0001 C CNN "SPR"
F 8 "" H 700 2700 50  0001 C CNN "SPN"
F 9 "" H 700 2700 50  0001 C CNN "SPURL"
	1    7750 5400
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP203
U 1 1 590FCE53
P 2600 5850
F 0 "JP203" H 2600 5930 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2610 5790 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 2600 5850 50  0001 C CNN
F 3 "" H 2600 5850 50  0001 C CNN
	1    2600 5850
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP204
U 1 1 590FCE59
P 2600 6050
F 0 "JP204" H 2600 6130 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2610 5990 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 2600 6050 50  0001 C CNN
F 3 "" H 2600 6050 50  0001 C CNN
	1    2600 6050
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP205
U 1 1 590FCE5F
P 3850 5850
F 0 "JP205" H 3850 5930 50  0000 C CNN
F 1 "Jumper_NO_Small" H 3860 5790 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 3850 5850 50  0001 C CNN
F 3 "" H 3850 5850 50  0001 C CNN
	1    3850 5850
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP206
U 1 1 590FCE65
P 3850 6050
F 0 "JP206" H 3850 6130 50  0000 C CNN
F 1 "Jumper_NO_Small" H 3860 5990 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 3850 6050 50  0001 C CNN
F 3 "" H 3850 6050 50  0001 C CNN
	1    3850 6050
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP209
U 1 1 590FCE6B
P 5150 5850
F 0 "JP209" H 5150 5930 50  0000 C CNN
F 1 "Jumper_NO_Small" H 5160 5790 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 5150 5850 50  0001 C CNN
F 3 "" H 5150 5850 50  0001 C CNN
	1    5150 5850
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP210
U 1 1 590FCE71
P 5150 6050
F 0 "JP210" H 5150 6130 50  0000 C CNN
F 1 "Jumper_NO_Small" H 5160 5990 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 5150 6050 50  0001 C CNN
F 3 "" H 5150 6050 50  0001 C CNN
	1    5150 6050
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP213
U 1 1 590FCE77
P 6400 5850
F 0 "JP213" H 6400 5930 50  0000 C CNN
F 1 "Jumper_NO_Small" H 6410 5790 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 6400 5850 50  0001 C CNN
F 3 "" H 6400 5850 50  0001 C CNN
	1    6400 5850
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP214
U 1 1 590FCE7D
P 6400 6050
F 0 "JP214" H 6400 6130 50  0000 C CNN
F 1 "Jumper_NO_Small" H 6410 5990 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 6400 6050 50  0001 C CNN
F 3 "" H 6400 6050 50  0001 C CNN
	1    6400 6050
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP217
U 1 1 590FCE83
P 7750 5850
F 0 "JP217" H 7750 5930 50  0000 C CNN
F 1 "Jumper_NO_Small" H 7760 5790 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 7750 5850 50  0001 C CNN
F 3 "" H 7750 5850 50  0001 C CNN
	1    7750 5850
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP218
U 1 1 590FCE89
P 7750 6050
F 0 "JP218" H 7750 6130 50  0000 C CNN
F 1 "Jumper_NO_Small" H 7760 5990 50  0000 C CNN
F 2 "Borgel Custom:SOLDER-JUMPER_1-WAY" H 7750 6050 50  0001 C CNN
F 3 "" H 7750 6050 50  0001 C CNN
	1    7750 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 5400 2150 5400
Wire Wire Line
	1700 5550 2150 5550
Wire Wire Line
	6850 5400 7300 5400
Wire Wire Line
	6850 5550 7300 5550
Wire Wire Line
	5600 5400 5950 5400
Wire Wire Line
	5600 5550 5950 5550
Wire Wire Line
	4300 5400 4700 5400
Wire Wire Line
	4300 5550 4700 5550
Wire Wire Line
	3050 5400 3400 5400
Wire Wire Line
	3050 5550 3400 5550
Wire Wire Line
	8300 5250 8200 5250
Wire Wire Line
	3100 6350 3100 5250
Wire Wire Line
	3100 5250 3050 5250
Wire Wire Line
	4800 4700 4800 4850
Wire Wire Line
	2050 4850 7300 4850
Wire Wire Line
	2050 4850 2050 5250
Wire Wire Line
	2050 5250 2150 5250
Wire Wire Line
	5950 5250 5900 5250
Wire Wire Line
	4300 5250 4500 5250
Wire Wire Line
	5600 5250 5800 5250
Wire Wire Line
	6850 5250 7100 5250
Connection ~ 4800 6350
Connection ~ 4800 4850
Wire Wire Line
	2100 5400 2100 5850
Wire Wire Line
	2100 5850 2500 5850
Connection ~ 2100 5400
Wire Wire Line
	2700 5850 3150 5850
Wire Wire Line
	2500 6050 2050 6050
Wire Wire Line
	2050 6050 2050 5550
Connection ~ 2050 5550
Wire Wire Line
	2700 6050 3200 6050
Wire Wire Line
	8200 5550 8800 5550
Wire Wire Line
	8200 5400 8800 5400
Wire Wire Line
	7300 4850 7300 5250
Wire Wire Line
	8300 6350 8300 5250
Wire Wire Line
	3100 6350 8300 6350
Wire Wire Line
	7100 5250 7100 6350
Connection ~ 7100 6350
Wire Wire Line
	7650 5850 7250 5850
Wire Wire Line
	7250 5850 7250 5400
Connection ~ 7250 5400
Wire Wire Line
	7200 5550 7200 6050
Wire Wire Line
	7200 6050 7650 6050
Connection ~ 7200 5550
Wire Wire Line
	7850 5850 8250 5850
Wire Wire Line
	8250 5850 8250 5400
Connection ~ 8250 5400
Wire Wire Line
	8350 6050 7850 6050
Wire Wire Line
	6500 5850 6900 5850
Wire Wire Line
	6900 5850 6900 5400
Connection ~ 6900 5400
Wire Wire Line
	6500 6050 6950 6050
Wire Wire Line
	6950 6050 6950 5550
Connection ~ 6950 5550
Wire Wire Line
	3400 5250 3350 5250
Wire Wire Line
	3350 5250 3350 4850
Connection ~ 3350 4850
Wire Wire Line
	3150 5850 3150 5400
Connection ~ 3150 5400
Wire Wire Line
	3200 6050 3200 5550
Connection ~ 3200 5550
Wire Wire Line
	3750 6050 3300 6050
Wire Wire Line
	3300 6050 3300 5550
Connection ~ 3300 5550
Wire Wire Line
	3750 5850 3350 5850
Wire Wire Line
	3350 5850 3350 5400
Connection ~ 3350 5400
Wire Wire Line
	4700 5250 4600 5250
Wire Wire Line
	4600 5250 4600 4850
Connection ~ 4600 4850
Wire Wire Line
	4500 5250 4500 6350
Connection ~ 4500 6350
Wire Wire Line
	5800 5250 5800 6350
Connection ~ 5800 6350
Wire Wire Line
	5900 5250 5900 4850
Connection ~ 5900 4850
Wire Wire Line
	5050 5850 4650 5850
Wire Wire Line
	4650 5850 4650 5400
Connection ~ 4650 5400
Wire Wire Line
	4600 5550 4600 6050
Wire Wire Line
	4600 6050 5050 6050
Connection ~ 4600 5550
Wire Wire Line
	5250 5850 5650 5850
Wire Wire Line
	5650 5850 5650 5400
Connection ~ 5650 5400
Wire Wire Line
	5250 6050 5700 6050
Wire Wire Line
	5700 6050 5700 5550
Connection ~ 5700 5550
Wire Wire Line
	6300 5850 5900 5850
Wire Wire Line
	5900 5850 5900 5400
Connection ~ 5900 5400
Wire Wire Line
	5850 5550 5850 6050
Wire Wire Line
	5850 6050 6300 6050
Connection ~ 5850 5550
Wire Wire Line
	3950 5850 4350 5850
Wire Wire Line
	4350 5850 4350 5400
Connection ~ 4350 5400
Wire Wire Line
	3950 6050 4400 6050
Wire Wire Line
	4400 6050 4400 5550
Connection ~ 4400 5550
Wire Wire Line
	8350 6050 8350 5550
Connection ~ 8350 5550
$EndSCHEMATC
