EESchema Schematic File Version 2
LIBS:78LC33
LIBS:ir2127
LIBS:ucc2732x
LIBS:dsei2x101
LIBS:q_nmos_dgss
LIBS:ZS17-cache
LIBS:+18v
LIBS:MCP2515-SOIC18
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:atmega328p
LIBS:atmega328p-au_alt
LIBS:atmega_can_shield-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title "ATMEGA CAN SHIELD"
Date "2018-04-19"
Rev "João A.C."
Comp "Zênite Solar"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6250 4550 6250 3950
Wire Wire Line
	5450 3550 5450 4550
Connection ~ 5450 3350
Wire Wire Line
	3100 3150 5450 3150
Wire Wire Line
	3100 3050 5450 3050
Wire Wire Line
	4900 4550 4900 3250
Connection ~ 5450 3750
Wire Wire Line
	5450 3350 5450 3450
Wire Wire Line
	4900 3250 5450 3250
Text Label 5000 3350 0    60   ~ 0
CAN_18V
Text Label 4950 3250 0    60   ~ 0
CAN_GND
Text Label 4600 3150 0    60   ~ 0
CAN_L
Text Label 4600 3050 0    60   ~ 0
CAN_H
Wire Wire Line
	8150 4550 8150 3950
Wire Wire Line
	7350 3550 7350 4550
Connection ~ 7350 3350
Wire Wire Line
	6500 3150 7350 3150
Wire Wire Line
	7350 3050 6500 3050
Wire Wire Line
	6800 4550 6800 3250
Connection ~ 7350 3750
Wire Wire Line
	7350 3350 7350 3450
Wire Wire Line
	6900 3350 7350 3350
Wire Wire Line
	6800 3250 7350 3250
Text Label 6900 3350 0    60   ~ 0
CAN_18V
Text Label 6850 3250 0    60   ~ 0
CAN_GND
Text Label 6500 3150 0    60   ~ 0
CAN_L
Text Label 6500 3050 0    60   ~ 0
CAN_H
$Comp
L RJ45 J402
U 1 1 5A01AFBA
P 7800 3400
F 0 "J402" H 8000 3900 50  0000 C CNN
F 1 "RJ45" H 7650 3900 50  0000 C CNN
F 2 "zenitesolar:RJ45_YH59_01" H 7800 3400 50  0001 C CNN
F 3 "http://www.assmann-wsw.com/fileadmin/datasheets/ASS_7039_CO.pdf" H 7800 3400 50  0001 C CNN
	1    7800 3400
	0    1    1    0   
$EndComp
Connection ~ 7350 4550
Wire Wire Line
	3100 4550 8150 4550
Connection ~ 5450 4550
Connection ~ 4900 4550
Connection ~ 6250 4550
Connection ~ 6800 4550
Text HLabel 3100 3050 0    60   BiDi ~ 0
CAN_H
Text HLabel 3100 3150 0    60   BiDi ~ 0
CAN_L
Text HLabel 3100 3350 0    60   Output ~ 0
CAN_18V
Text HLabel 3100 4550 0    60   BiDi ~ 0
CAN_GND
Wire Wire Line
	3100 3350 5450 3350
$Comp
L RJ45 J401
U 1 1 5A01AFA3
P 5900 3400
F 0 "J401" H 6100 3900 50  0000 C CNN
F 1 "RJ45" H 5750 3900 50  0000 C CNN
F 2 "zenitesolar:RJ45_YH59_01" H 5900 3400 50  0001 C CNN
F 3 "http://www.assmann-wsw.com/fileadmin/datasheets/ASS_7039_CO.pdf" H 5900 3400 50  0001 C CNN
	1    5900 3400
	0    1    1    0   
$EndComp
Connection ~ 5450 3650
Connection ~ 7350 3650
$Comp
L PWR_FLAG #FLG030
U 1 1 5ADC43CE
P 3450 3400
F 0 "#FLG030" H 3450 3475 50  0001 C CNN
F 1 "PWR_FLAG" H 3450 3550 50  0000 C CNN
F 2 "" H 3450 3400 50  0001 C CNN
F 3 "" H 3450 3400 50  0001 C CNN
	1    3450 3400
	-1   0    0    1   
$EndComp
$Comp
L PWR_FLAG #FLG031
U 1 1 5ADC4406
P 3450 4500
F 0 "#FLG031" H 3450 4575 50  0001 C CNN
F 1 "PWR_FLAG" H 3450 4650 50  0000 C CNN
F 2 "" H 3450 4500 50  0001 C CNN
F 3 "" H 3450 4500 50  0001 C CNN
	1    3450 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 3400 3450 3350
Connection ~ 3450 3350
Wire Wire Line
	3450 4500 3450 4550
Connection ~ 3450 4550
$EndSCHEMATC
