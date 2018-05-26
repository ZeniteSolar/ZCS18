EESchema Schematic File Version 4
LIBS:atmega_can_shield-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
Title "ATMEGA CAN SHIELD"
Date "2018-04-19"
Rev "João A.C."
Comp "Zênite Solar"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ZS17-cache:MCP2551-I_SN U201
U 1 1 59FDAEE3
P 6450 3300
F 0 "U201" H 6050 3650 50  0000 L CNN
F 1 "MCP2561" H 6500 2950 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 6450 2800 50  0001 C CIN
F 3 "https://www.mouser.com/datasheet/2/268/25167B-245700.pdf" H 6450 3300 50  0001 C CNN
F 4 "https://www.mouser.com/ProductDetail/Microchip-Technology/MCP2561T-E-SN?qs=sGAEpiMZZMuyKkoWRCJ2WCPSW63XBcAp862Nn1F%252bVFQ%3d" H 6450 3300 50  0001 C CNN "Mouser"
	1    6450 3300
	1    0    0    -1  
$EndComp
Text Label 5250 3200 2    60   ~ 0
TXCAN
Text Label 5250 3100 2    60   ~ 0
RXCAN
$Comp
L ZS17-cache:MCP2515-E_ST-SOIC18 U202
U 1 1 59FDAF0B
P 4300 3700
F 0 "U202" H 4150 4450 50  0000 R CNN
F 1 "MCP2515-E/ST-SOIC18" H 5300 2900 50  0000 R TNN
F 2 "Housings_SOIC:SOIC-18W_7.5x11.6mm_Pitch1.27mm" H 4300 2800 50  0001 C CIN
F 3 "https://www.mouser.com/datasheet/2/268/20001801H-708845.pdf" H 4400 2900 50  0001 C CNN
F 4 "https://www.mouser.com/ProductDetail/Microchip-Technology/MCP2515T-I-SO?qs=sGAEpiMZZMuyKkoWRCJ2WHe%252bbxCW0ve6xjnnyKAx3CA%3d" H 4300 3700 50  0001 C CNN "Mouser"
	1    4300 3700
	1    0    0    -1  
$EndComp
NoConn ~ 3700 4100
NoConn ~ 4900 3800
NoConn ~ 4900 3900
NoConn ~ 4900 4000
NoConn ~ 4900 4100
NoConn ~ 4900 4200
$Comp
L ZS17-cache:R_Small R206
U 1 1 59FDAF39
P 5100 4300
F 0 "R206" V 4904 4300 50  0000 C CNN
F 1 "10k" V 4995 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5100 4300 50  0001 C CNN
F 3 "" H 5100 4300 50  0001 C CNN
	1    5100 4300
	0    1    1    0   
$EndComp
$Comp
L ZS17-cache:R_Small R203
U 1 1 59FDAF40
P 5800 3750
F 0 "R203" H 5830 3770 50  0000 L CNN
F 1 "10k" H 5830 3710 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5800 3750 50  0001 C CNN
F 3 "" H 5800 3750 50  0001 C CNN
	1    5800 3750
	1    0    0    -1  
$EndComp
NoConn ~ 5950 3400
$Comp
L ZS17-cache:R_Small R201
U 1 1 59FDAF5C
P 8850 3600
F 0 "R201" V 8600 3600 50  0000 C CNN
F 1 "120" V 8700 3600 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 8850 3600 50  0001 C CNN
F 3 "" H 8850 3600 50  0001 C CNN
	1    8850 3600
	0    1    1    0   
$EndComp
Text HLabel 2950 3100 0    60   Input ~ 0
SI
Text HLabel 2950 3200 0    60   Output ~ 0
SO
Text HLabel 2950 3300 0    60   Input ~ 0
CS
Text HLabel 2950 3400 0    60   Input ~ 0
SCK
Text HLabel 5000 3700 2    60   Output ~ 0
INT
Text HLabel 2050 5050 0    60   Output ~ 0
GND
Text HLabel 2050 2650 0    60   Input ~ 0
VCC
Text HLabel 9800 3200 2    60   BiDi ~ 0
CAN_H
Text HLabel 9800 3400 2    60   BiDi ~ 0
CAN_L
$Comp
L ZS17-cache:C_Small C202
U 1 1 5AAC0190
P 4450 2750
F 0 "C202" V 4221 2750 50  0000 C CNN
F 1 "100nF" V 4312 2750 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4450 2750 50  0001 C CNN
F 3 "" H 4450 2750 50  0001 C CNN
	1    4450 2750
	0    1    1    0   
$EndComp
$Comp
L ZS17-cache:GND #PWR019
U 1 1 5AAC0405
P 4600 2800
F 0 "#PWR019" H 4600 2550 50  0001 C CNN
F 1 "GND" H 4600 2650 50  0000 C CNN
F 2 "" H 4600 2800 50  0001 C CNN
F 3 "" H 4600 2800 50  0001 C CNN
	1    4600 2800
	1    0    0    -1  
$EndComp
$Comp
L ZS17-cache:C_Small C203
U 1 1 5AAC0546
P 6600 2750
F 0 "C203" V 6371 2750 50  0000 C CNN
F 1 "100nF" V 6462 2750 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6600 2750 50  0001 C CNN
F 3 "" H 6600 2750 50  0001 C CNN
	1    6600 2750
	0    1    1    0   
$EndComp
$Comp
L ZS17-cache:GND #PWR020
U 1 1 5AAC054E
P 6750 2800
F 0 "#PWR020" H 6750 2550 50  0001 C CNN
F 1 "GND" H 6750 2650 50  0000 C CNN
F 2 "" H 6750 2800 50  0001 C CNN
F 3 "" H 6750 2800 50  0001 C CNN
	1    6750 2800
	1    0    0    -1  
$EndComp
$Comp
L atmega_can_shield-rescue:D_TVS_x2_AAC D201
U 1 1 5AAC0839
P 9050 4000
F 0 "D201" H 9050 4216 50  0000 C CNN
F 1 "pesd1can" H 9050 4125 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 8900 4000 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/PESD1CAN.pdf" H 8900 4000 50  0001 C CNN
F 4 "https://www.mouser.com/ProductDetail/Nexperia/PESD1CAN-UX?qs=sGAEpiMZZMsoIvTP6m9%252bn17u26dJac3S" H 9050 4000 50  0001 C CNN "Mouser"
	1    9050 4000
	1    0    0    -1  
$EndComp
Text Notes 5650 6250 0    60   Italic 0
EMI/ESD Protection Solutions for the CAN Bus: \n	https://www.onsemi.com/pub/Collateral/AND8169-D.PDF\nSparkFun CAN-Bus Shield:\n	https://cdn.sparkfun.com/datasheets/Dev/Arduino/Shields/SparkFun_CAN-Bus_Shield_v13a.pdf\nMSP430 CAN INTERFACE:\n	http://www.electrodummies.net/en/msp430-2/msp430-can-interface/
$Comp
L atmega_can_shield-rescue:Jumper_NO_Small JP201
U 1 1 5AAC0FAA
P 9250 3600
F 0 "JP201" H 9250 3750 50  0000 C CNN
F 1 "Termination" H 9250 3650 50  0000 C CNN
F 2 "Connectors:GS2" H 9250 3600 50  0001 C CNN
F 3 "" H 9250 3600 50  0001 C CNN
	1    9250 3600
	1    0    0    -1  
$EndComp
$Comp
L atmega_can_shield-rescue:TEST TP203
U 1 1 5AAC2327
P 5550 3050
F 0 "TP203" H 5628 3190 50  0000 L CNN
F 1 "TXCAN" H 5628 3099 50  0000 L CNN
F 2 "Measurement_Points:Measurement_Point_Round-SMD-Pad_Small" H 5750 3050 50  0001 C CNN
F 3 "" H 5750 3050 50  0001 C CNN
	1    5550 3050
	1    0    0    -1  
$EndComp
$Comp
L atmega_can_shield-rescue:TEST TP204
U 1 1 5AAC23CC
P 5550 3250
F 0 "TP204" H 5472 3297 50  0000 R CNN
F 1 "RXCAN" H 5472 3388 50  0000 R CNN
F 2 "Measurement_Points:Measurement_Point_Round-SMD-Pad_Small" H 5750 3250 50  0001 C CNN
F 3 "" H 5750 3250 50  0001 C CNN
	1    5550 3250
	-1   0    0    1   
$EndComp
$Comp
L ZS17-cache:R_Small R204
U 1 1 5AAC5A25
P 8500 3750
F 0 "R204" H 8442 3796 50  0000 R CNN
F 1 "100" H 8442 3705 50  0000 R CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 8500 3750 50  0001 C CNN
F 3 "" H 8500 3750 50  0001 C CNN
	1    8500 3750
	1    0    0    -1  
$EndComp
$Comp
L ZS17-cache:R_Small R205
U 1 1 5AAC5BC6
P 9600 3750
F 0 "R205" H 9659 3796 50  0000 L CNN
F 1 "100" H 9659 3705 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 9600 3750 50  0001 C CNN
F 3 "" H 9600 3750 50  0001 C CNN
	1    9600 3750
	1    0    0    -1  
$EndComp
$Comp
L ZS17-cache:C_Small C204
U 1 1 5AAC5D22
P 8500 4050
F 0 "C204" H 8591 4004 50  0000 L CNN
F 1 "560p" H 8591 4095 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8500 4050 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/447/UPY-GPHC_X7R_6.3V-to-50V_18-1154002.pdf" H 8500 4050 50  0001 C CNN
F 4 "https://br.mouser.com/ProductDetail/Yageo/CC0805KRX7R9BB561?qs=sGAEpiMZZMs0AnBnWHyRQIOKs%2fDjbJvGAqQC1zE3t74%3d" H 8500 4050 50  0001 C CNN "Mouser"
	1    8500 4050
	-1   0    0    1   
$EndComp
$Comp
L ZS17-cache:GND #PWR021
U 1 1 5AAC605D
P 9050 4250
F 0 "#PWR021" H 9050 4000 50  0001 C CNN
F 1 "GND" H 9050 4100 50  0000 C CNN
F 2 "" H 9050 4250 50  0001 C CNN
F 3 "" H 9050 4250 50  0001 C CNN
	1    9050 4250
	1    0    0    -1  
$EndComp
$Comp
L atmega_can_shield-rescue:TEST TP208
U 1 1 5AAC8491
P 9650 3600
F 0 "TP208" V 9604 3828 50  0000 L CNN
F 1 "CAN_L" V 9695 3828 50  0000 L CNN
F 2 "Measurement_Points:Measurement_Point_Round-SMD-Pad_Small" H 9850 3600 50  0001 C CNN
F 3 "" H 9850 3600 50  0001 C CNN
	1    9650 3600
	0    1    1    0   
$EndComp
$Comp
L atmega_can_shield-rescue:TEST TP207
U 1 1 5AAC8C25
P 8450 3600
F 0 "TP207" V 8404 3828 50  0000 L CNN
F 1 "CAN_H" V 8495 3828 50  0000 L CNN
F 2 "Measurement_Points:Measurement_Point_Round-SMD-Pad_Small" H 8650 3600 50  0001 C CNN
F 3 "" H 8650 3600 50  0001 C CNN
	1    8450 3600
	0    -1   1    0   
$EndComp
Text Notes 6500 4050 0    39   ~ 0
(Alt. Parts: \n	TJA1050, \n	TJA1051,\n	MCP2551)
$Comp
L ZS17-cache:Crystal_Small Y201
U 1 1 5AD9E2C0
P 2900 4150
F 0 "Y201" H 2900 3950 50  0000 C CNN
F 1 "16MHz" H 2900 4050 50  0000 C CNN
F 2 "zenitesolar:Crystal_SMD_HC49_4HSMX" H 2900 4150 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/321153.pdf" H 2900 4150 50  0001 C CNN
F 4 "https://www.mouser.com/ProductDetail/IQD/LFXTAL003237Reel?qs=sGAEpiMZZMsBj6bBr9Q9af1kE%252bXo19x3%252bSVT%252b2%252bBEMZebQOWriiNcg%3d%3d" H 2900 4150 50  0001 C CNN "Mouser"
	1    2900 4150
	-1   0    0    1   
$EndComp
$Comp
L ZS17-cache:C_Small C207
U 1 1 5AD9E2C7
P 3150 4350
F 0 "C207" H 3242 4396 50  0000 L CNN
F 1 "22pF" H 3242 4305 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3150 4350 50  0001 C CNN
F 3 "" H 3150 4350 50  0001 C CNN
	1    3150 4350
	1    0    0    -1  
$EndComp
$Comp
L ZS17-cache:C_Small C206
U 1 1 5AD9E2CE
P 2650 4350
F 0 "C206" H 2559 4304 50  0000 R CNN
F 1 "22pF" H 2559 4395 50  0000 R CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2650 4350 50  0001 C CNN
F 3 "" H 2650 4350 50  0001 C CNN
	1    2650 4350
	1    0    0    1   
$EndComp
Text Label 3500 3900 2    60   ~ 0
CLK_P
Text Label 3500 4000 2    60   ~ 0
CLK_N
$Comp
L power1:GNDD #PWR022
U 1 1 5AD9E2D7
P 2900 4650
F 0 "#PWR022" H 2900 4400 50  0001 C CNN
F 1 "GNDD" H 2900 4525 50  0000 C CNN
F 2 "" H 2900 4650 50  0001 C CNN
F 3 "" H 2900 4650 50  0001 C CNN
	1    2900 4650
	-1   0    0    -1  
$EndComp
$Comp
L ZS17-cache:GND #PWR023
U 1 1 5AD9EE4B
P 2250 5100
F 0 "#PWR023" H 2250 4850 50  0001 C CNN
F 1 "GND" H 2250 4950 50  0000 C CNN
F 2 "" H 2250 5100 50  0001 C CNN
F 3 "" H 2250 5100 50  0001 C CNN
	1    2250 5100
	1    0    0    -1  
$EndComp
$Comp
L atmega_can_shield-rescue:L_Core_Ferrite L201
U 1 1 5ADC3336
P 4050 2650
F 0 "L201" V 4000 2650 50  0000 C CNN
F 1 "10uH" V 4160 2650 50  0000 C CNN
F 2 "Inductors_SMD:L_0805_HandSoldering" H 4050 2650 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/400/nductor_commercial_decoupling_mlz2012_en-918301.pdf" H 4050 2650 50  0001 C CNN
F 4 "https://www.mouser.com/ProductDetail/TDK/MLZ2012N100L?qs=sGAEpiMZZMsg%252by3WlYCkUxULy5EXuT33zQWWbOu0YZs%3d" V 4050 2650 50  0001 C CNN "Mouser"
	1    4050 2650
	0    1    1    0   
$EndComp
Text Notes 2450 3850 0    39   ~ 0
GNDD Guard
$Comp
L ZS17-cache:R_Small R202
U 1 1 5AD8F0BD
P 5600 3750
F 0 "R202" H 5630 3770 50  0000 L CNN
F 1 "10k" H 5630 3710 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5600 3750 50  0001 C CNN
F 3 "" H 5600 3750 50  0001 C CNN
	1    5600 3750
	-1   0    0    -1  
$EndComp
$Comp
L ZS17-cache:C_Small C201
U 1 1 5ADA1382
P 3750 2400
F 0 "C201" V 3979 2400 50  0000 C CNN
F 1 "100nF" V 3888 2400 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3750 2400 50  0001 C CNN
F 3 "" H 3750 2400 50  0001 C CNN
	1    3750 2400
	0    -1   -1   0   
$EndComp
$Comp
L ZS17-cache:GND #PWR024
U 1 1 5ADA1D4D
P 3650 2450
F 0 "#PWR024" H 3650 2200 50  0001 C CNN
F 1 "GND" H 3650 2300 50  0000 C CNN
F 2 "" H 3650 2450 50  0001 C CNN
F 3 "" H 3650 2450 50  0001 C CNN
	1    3650 2450
	1    0    0    -1  
$EndComp
$Comp
L atmega_can_shield-rescue:GS2 J201
U 1 1 5ADA43DD
P 3150 4800
F 0 "J201" H 3218 4846 50  0000 L CNN
F 1 "NetTie" H 3218 4755 50  0000 L CNN
F 2 "zenitesolar:µNetTie-I_Connected_SMD" V 3224 4800 50  0001 C CNN
F 3 "" H 3150 4800 50  0001 C CNN
	1    3150 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 4300 5000 4300
Wire Wire Line
	5300 4300 5200 4300
Wire Wire Line
	5800 3500 5800 3600
Wire Wire Line
	5800 3500 5950 3500
Wire Wire Line
	6450 2650 6450 2750
Wire Wire Line
	4300 5050 4300 4500
Wire Wire Line
	6450 5050 6450 3700
Wire Wire Line
	5800 5050 5800 3900
Connection ~ 5800 5050
Connection ~ 4300 5050
Connection ~ 5300 2650
Wire Wire Line
	5300 2650 5300 4300
Wire Wire Line
	2050 5050 2250 5050
Wire Wire Line
	4900 3100 5250 3100
Wire Wire Line
	5250 3100 5350 3200
Wire Wire Line
	4900 3200 5250 3200
Wire Wire Line
	5250 3200 5350 3100
Wire Wire Line
	5350 3100 5550 3100
Wire Wire Line
	5350 3200 5550 3200
Wire Wire Line
	4300 2750 4350 2750
Connection ~ 4300 2750
Wire Wire Line
	4550 2750 4600 2750
Wire Wire Line
	4600 2750 4600 2800
Wire Wire Line
	6700 2750 6750 2750
Wire Wire Line
	6750 2750 6750 2800
Wire Wire Line
	6500 2750 6450 2750
Connection ~ 6450 2750
Wire Wire Line
	9050 4150 9050 4200
Wire Wire Line
	9150 3600 8950 3600
Wire Wire Line
	5550 3050 5550 3100
Connection ~ 5550 3100
Wire Wire Line
	5550 3250 5550 3200
Connection ~ 5550 3200
Wire Wire Line
	8500 3950 8500 3850
Wire Wire Line
	9600 3850 9600 3950
Wire Wire Line
	9600 4200 9600 4150
Wire Wire Line
	8500 4200 9050 4200
Wire Wire Line
	8500 4200 8500 4150
Connection ~ 9050 4200
Wire Wire Line
	8500 3200 8500 3600
Wire Wire Line
	9600 3400 9600 3600
Wire Wire Line
	9400 4000 9400 3600
Wire Wire Line
	9350 3600 9400 3600
Connection ~ 9600 3600
Wire Wire Line
	8700 3600 8700 4000
Wire Wire Line
	8450 3600 8500 3600
Connection ~ 8500 3600
Connection ~ 8700 3600
Connection ~ 9400 3600
Wire Wire Line
	2650 3900 3700 3900
Wire Wire Line
	2650 3900 2650 4150
Wire Wire Line
	2650 4150 2800 4150
Wire Wire Line
	3000 4150 3150 4150
Wire Wire Line
	3150 4000 3150 4150
Wire Wire Line
	3150 4450 3150 4550
Wire Wire Line
	2650 4550 2650 4450
Connection ~ 2900 4550
Wire Wire Line
	3150 4000 3700 4000
Connection ~ 3150 4150
Connection ~ 2650 4150
Wire Wire Line
	2900 4500 2900 4550
Connection ~ 3150 5050
Connection ~ 3150 4550
Wire Wire Line
	2250 5100 2250 5050
Connection ~ 2250 5050
Wire Wire Line
	4200 2650 4300 2650
Wire Wire Line
	4300 2600 4300 2650
Connection ~ 4300 2650
Wire Wire Line
	2050 2650 3850 2650
Wire Notes Line
	2450 3850 2450 5150
Wire Notes Line
	2450 5150 3550 5150
Wire Notes Line
	3550 5150 3550 3850
Wire Notes Line
	3550 3850 2450 3850
Wire Wire Line
	5800 3600 5600 3600
Wire Wire Line
	5600 3600 5600 3650
Connection ~ 5800 3600
Wire Wire Line
	5600 3850 5600 3900
Wire Wire Line
	5600 3900 5800 3900
Connection ~ 5800 3900
Wire Notes Line
	2450 4650 2750 4650
Wire Notes Line
	2750 4650 2750 4550
Wire Wire Line
	3850 2400 3850 2650
Connection ~ 3850 2650
Wire Wire Line
	3650 2400 3650 2450
Wire Wire Line
	3150 5000 3150 5050
Wire Wire Line
	3150 4550 2900 4550
$Comp
L power1:PWR_FLAG #FLG025
U 1 1 5ADC2450
P 4300 2600
F 0 "#FLG025" H 4300 2675 50  0001 C CNN
F 1 "PWR_FLAG" H 4300 2774 50  0000 C CNN
F 2 "" H 4300 2600 50  0001 C CNN
F 3 "" H 4300 2600 50  0001 C CNN
	1    4300 2600
	1    0    0    -1  
$EndComp
$Comp
L power1:PWR_FLAG #FLG026
U 1 1 5ADEB7DF
P 2900 4500
F 0 "#FLG026" H 2900 4575 50  0001 C CNN
F 1 "PWR_FLAG" H 2900 4650 39  0000 C CNN
F 2 "" H 2900 4500 50  0001 C CNN
F 3 "" H 2900 4500 50  0001 C CNN
	1    2900 4500
	1    0    0    -1  
$EndComp
Text Label 4850 2650 0    30   ~ 0
local_VCC
Wire Wire Line
	5800 5050 6450 5050
Wire Wire Line
	4300 5050 5800 5050
Wire Wire Line
	5300 2650 6450 2650
Wire Wire Line
	4300 2750 4300 2900
Wire Wire Line
	6450 2750 6450 2900
Wire Wire Line
	5550 3100 5950 3100
Wire Wire Line
	5550 3200 5950 3200
Wire Wire Line
	9050 4200 9050 4250
Wire Wire Line
	9050 4200 9600 4200
Wire Wire Line
	8500 3200 9800 3200
Wire Wire Line
	9600 3400 9800 3400
Wire Wire Line
	9600 3600 9600 3650
Wire Wire Line
	9600 3600 9650 3600
Wire Wire Line
	8500 3600 8500 3650
Wire Wire Line
	8500 3600 8700 3600
Wire Wire Line
	8700 3600 8750 3600
Wire Wire Line
	9400 3600 9600 3600
Wire Wire Line
	2900 4550 2900 4650
Wire Wire Line
	2900 4550 2650 4550
Wire Wire Line
	3150 4150 3150 4250
Wire Wire Line
	2650 4150 2650 4250
Wire Wire Line
	3150 5050 4300 5050
Wire Wire Line
	3150 4550 3150 4600
Wire Wire Line
	2250 5050 3150 5050
Wire Wire Line
	4300 2650 5300 2650
Wire Wire Line
	4300 2650 4300 2750
Wire Wire Line
	5800 3600 5800 3650
Wire Wire Line
	5800 3900 5800 3850
Wire Wire Line
	3850 2650 3900 2650
Wire Wire Line
	2950 3400 3700 3400
Wire Wire Line
	2950 3300 3700 3300
Wire Wire Line
	2950 3200 3700 3200
Wire Wire Line
	2950 3100 3700 3100
Wire Wire Line
	4900 3700 5000 3700
Connection ~ 9600 3400
Connection ~ 8500 3200
Wire Wire Line
	6950 3200 7300 3200
Wire Wire Line
	6950 3400 7300 3400
$Comp
L Jumper:Jumper_2_Open JP202
U 1 1 5AEDD6EC
P 7500 3200
F 0 "JP202" H 7500 3435 50  0000 C CNN
F 1 "Jumper_2_Open" H 7500 3344 50  0000 C CNN
F 2 "Resistors_SMD:R_0402_NoSilk" H 7500 3200 50  0001 C CNN
F 3 "~" H 7500 3200 50  0001 C CNN
	1    7500 3200
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP203
U 1 1 5AEDDF3F
P 7500 3400
F 0 "JP203" H 7500 3600 50  0000 C CNN
F 1 "Jumper_2_Open" H 7500 3500 50  0000 C CNN
F 2 "Resistors_SMD:R_0402_NoSilk" H 7500 3400 50  0001 C CNN
F 3 "~" H 7500 3400 50  0001 C CNN
	1    7500 3400
	1    0    0    1   
$EndComp
Wire Wire Line
	7700 3400 9600 3400
Wire Wire Line
	7700 3200 8500 3200
$Comp
L ZS17-cache:C_Small C205
U 1 1 5AAC5E7F
P 9600 4050
F 0 "C205" H 9692 4004 50  0000 L CNN
F 1 "560p" H 9692 4095 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 9600 4050 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/447/UPY-GPHC_X7R_6.3V-to-50V_18-1154002.pdf" H 9600 4050 50  0001 C CNN
F 4 "https://br.mouser.com/ProductDetail/Yageo/CC0805KRX7R9BB561?qs=sGAEpiMZZMs0AnBnWHyRQIOKs%2fDjbJvGAqQC1zE3t74%3d" H 9600 4050 50  0001 C CNN "Mouser"
	1    9600 4050
	1    0    0    1   
$EndComp
$EndSCHEMATC
