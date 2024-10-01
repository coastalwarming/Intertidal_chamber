EESchema Schematic File Version 4
EELAYER 30 0
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
Text Notes 3715 2035 2    0    ~ 0
wewew
$Comp
L lsts-ic:MCP23017 IC?
U 1 1 6209A6FC
P 4275 2175
AR Path="/61AA1D44/6209A6FC" Ref="IC?"  Part="1" 
AR Path="/62098590/6209A6FC" Ref="IC2"  Part="1" 
AR Path="/621C3274/6209A6FC" Ref="IC1"  Part="1" 
F 0 "IC1" H 4375 2776 39  0000 C CNN
F 1 "MCP23017" H 4375 2701 39  0000 C CNN
F 2 "Footprints_Luis:MCP23017-E_SP" H 4275 2175 60  0001 C CNN
F 3 "" H 4275 2175 60  0000 C CNN
	1    4275 2175
	1    0    0    -1  
$EndComp
Text Label 3760 2875 2    50   ~ 0
I2C_SCl
Wire Wire Line
	3760 2875 3825 2875
Text Label 3760 2975 2    50   ~ 0
I2C_SDA
Wire Wire Line
	3760 2975 3825 2975
Wire Wire Line
	4995 1975 4925 1975
Wire Wire Line
	4995 1875 4925 1875
Wire Wire Line
	4995 1775 4925 1775
Wire Wire Line
	3755 2475 3825 2475
Wire Wire Line
	3755 2375 3825 2375
Wire Wire Line
	3755 2275 3825 2275
Wire Wire Line
	3755 2175 3825 2175
Wire Wire Line
	3755 2075 3825 2075
Wire Wire Line
	3755 1975 3825 1975
Wire Wire Line
	3755 1875 3825 1875
Wire Wire Line
	3755 1775 3825 1775
$Comp
L power:GND #PWR?
U 1 1 6209A726
P 4925 3005
AR Path="/61AA1D44/6209A726" Ref="#PWR?"  Part="1" 
AR Path="/62098590/6209A726" Ref="#PWR023"  Part="1" 
AR Path="/621C3274/6209A726" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 4925 3005 30  0001 C CNN
F 1 "GND" H 4925 2935 30  0001 C CNN
F 2 "" H 4925 3005 60  0000 C CNN
F 3 "" H 4925 3005 60  0000 C CNN
	1    4925 3005
	1    0    0    -1  
$EndComp
Wire Wire Line
	4925 2975 4925 2875
Connection ~ 4925 2975
$Comp
L power1:+3.3V #PWR?
U 1 1 6209A731
P 5555 2695
AR Path="/61AA1D44/6209A731" Ref="#PWR?"  Part="1" 
AR Path="/62098590/6209A731" Ref="#PWR024"  Part="1" 
AR Path="/621C3274/6209A731" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 5555 2545 50  0001 C CNN
F 1 "+3.3V" H 5570 2868 50  0000 C CNN
F 2 "" H 5555 2695 50  0001 C CNN
F 3 "" H 5555 2695 50  0001 C CNN
	1    5555 2695
	1    0    0    -1  
$EndComp
$Comp
L power1:+3.3V #PWR?
U 1 1 6209A737
P 3020 2575
AR Path="/61AA1D44/6209A737" Ref="#PWR?"  Part="1" 
AR Path="/62098590/6209A737" Ref="#PWR021"  Part="1" 
AR Path="/621C3274/6209A737" Ref="#PWR015"  Part="1" 
F 0 "#PWR015" H 3020 2425 50  0001 C CNN
F 1 "+3.3V" H 2860 2675 50  0000 C CNN
F 2 "" H 3020 2575 50  0001 C CNN
F 3 "" H 3020 2575 50  0001 C CNN
	1    3020 2575
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6209A73E
P 3020 2790
AR Path="/61AA1D44/6209A73E" Ref="#PWR?"  Part="1" 
AR Path="/62098590/6209A73E" Ref="#PWR022"  Part="1" 
AR Path="/621C3274/6209A73E" Ref="#PWR016"  Part="1" 
F 0 "#PWR016" H 3020 2790 30  0001 C CNN
F 1 "GND" H 3020 2720 30  0001 C CNN
F 2 "" H 3020 2790 60  0000 C CNN
F 3 "" H 3020 2790 60  0000 C CNN
	1    3020 2790
	1    0    0    -1  
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 6209A745
P 5305 2775
AR Path="/61AA1D44/6209A745" Ref="R?"  Part="1" 
AR Path="/62098590/6209A745" Ref="R51"  Part="1" 
AR Path="/621C3274/6209A745" Ref="R21"  Part="1" 
F 0 "R21" V 5097 2775 50  0000 C CNN
F 1 "10k" V 5188 2775 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 5305 2775 60  0001 C CNN
F 3 "" H 5305 2775 60  0000 C CNN
	1    5305 2775
	0    1    1    0   
$EndComp
Wire Wire Line
	4925 2775 5035 2775
NoConn ~ 4925 2575
NoConn ~ 4925 2675
Wire Wire Line
	3825 2675 3635 2675
Wire Wire Line
	3635 2675 3635 2725
Wire Wire Line
	5555 2695 5555 2775
Wire Wire Line
	3635 2775 3825 2775
Text Label 3755 2075 2    50   ~ 0
Tide_Manual_LOW
Text Label 3755 1975 2    50   ~ 0
Tide_Manual_HIGH
NoConn ~ 3825 3075
Wire Wire Line
	4925 3005 4925 2975
Wire Wire Line
	4925 3075 5035 3075
Wire Wire Line
	5035 3075 5035 2775
Connection ~ 5035 2775
Wire Wire Line
	5035 2775 5055 2775
$Comp
L Simbolos_Luis:SW_SPDT SW7
U 1 1 620C394D
P 3090 5160
F 0 "SW7" H 3090 5445 50  0000 C CNN
F 1 "SW_SPDT" H 3090 5354 50  0000 C CNN
F 2 "Footprints_Luis:SW_100SP1T1B4M2QE" H 3090 5160 50  0001 C CNN
F 3 "1-1825137-6" H 3090 5160 50  0001 C CNN
	1    3090 5160
	1    0    0    -1  
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 62101C10
P 3370 5565
AR Path="/61AA1D44/62101C10" Ref="R?"  Part="1" 
AR Path="/62098590/62101C10" Ref="R53"  Part="1" 
AR Path="/621C3274/62101C10" Ref="R20"  Part="1" 
F 0 "R20" V 3420 5260 50  0000 C CNN
F 1 "10k" V 3365 5555 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 3370 5565 60  0001 C CNN
F 3 "" H 3370 5565 60  0000 C CNN
	1    3370 5565
	-1   0    0    1   
$EndComp
$Comp
L power1:+3.3V #PWR?
U 1 1 6210CDA8
P 2805 5145
AR Path="/61AA1D44/6210CDA8" Ref="#PWR?"  Part="1" 
AR Path="/62098590/6210CDA8" Ref="#PWR025"  Part="1" 
AR Path="/621C3274/6210CDA8" Ref="#PWR014"  Part="1" 
F 0 "#PWR014" H 2805 4995 50  0001 C CNN
F 1 "+3.3V" H 2700 5305 50  0000 C CNN
F 2 "" H 2805 5145 50  0001 C CNN
F 3 "" H 2805 5145 50  0001 C CNN
	1    2805 5145
	1    0    0    -1  
$EndComp
Wire Wire Line
	2805 5145 2805 5160
Wire Wire Line
	2805 5160 2890 5160
Text Label 3485 5060 0    50   ~ 0
Heat_Lamp_Manual_OFF
Text Label 2005 6880 0    50   ~ 0
FAN_Manual_ON
Text Label 1995 6680 0    50   ~ 0
FAN_Manual_OFF
Text Label 1955 5585 0    50   ~ 0
Light_Manual_ON
Text Label 1945 5385 0    50   ~ 0
Light_Manual_OFF
Text Label 1920 3930 0    50   ~ 0
Water_Cooler_Manual_OFF
Text Label 3755 2175 2    50   ~ 0
Heat_Lamp_Manual_OFF
Wire Wire Line
	3020 2575 3825 2575
Text Label 3755 2275 2    50   ~ 0
Water_Heat_Manual_OFF
Text Label 3755 2375 2    50   ~ 0
Water_Cooler_Manual_OFF
Text Label 4995 1775 0    50   ~ 0
FAN_Manual_ON
Text Label 3755 2475 2    50   ~ 0
FAN_Manual_OFF
Wire Wire Line
	3020 2790 3020 2725
Wire Wire Line
	3020 2725 3635 2725
Connection ~ 3635 2725
Wire Wire Line
	3635 2725 3635 2775
Text Label 4995 1975 0    50   ~ 0
Light_Manual_ON
Text Label 4995 1875 0    50   ~ 0
Light_Manual_OFF
Text Label 1255 800  0    50   ~ 0
I2C_SCL
Text Label 1255 900  0    50   ~ 0
I2C_SDA
Text HLabel 1080 800  0    50   Input ~ 0
I2C_SCl
Wire Wire Line
	1080 800  1255 800 
Text HLabel 1080 900  0    50   Input ~ 0
I2C_SDA
Wire Wire Line
	1080 900  1255 900 
Text Label 1850 1200 0    50   ~ 0
All_OFF
Wire Wire Line
	3290 5060 3370 5060
Wire Wire Line
	3370 5315 3370 5060
Connection ~ 3370 5060
Wire Wire Line
	3370 5060 3485 5060
$Comp
L Simbolos_Luis:SW_SPDT SW6
U 1 1 623FC97E
P 3070 6475
F 0 "SW6" H 3070 6760 50  0000 C CNN
F 1 "SW_SPDT" H 3070 6669 50  0000 C CNN
F 2 "Footprints_Luis:SW_100SP1T1B4M2QE" H 3070 6475 50  0001 C CNN
F 3 "1-1825137-6" H 3070 6475 50  0001 C CNN
	1    3070 6475
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 623FC984
P 3350 7340
AR Path="/61AA1D44/623FC984" Ref="#PWR?"  Part="1" 
AR Path="/62098590/623FC984" Ref="#PWR036"  Part="1" 
AR Path="/621C3274/623FC984" Ref="#PWR017"  Part="1" 
F 0 "#PWR017" H 3350 7340 30  0001 C CNN
F 1 "GND" H 3350 7270 30  0001 C CNN
F 2 "" H 3350 7340 60  0000 C CNN
F 3 "" H 3350 7340 60  0000 C CNN
	1    3350 7340
	1    0    0    -1  
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 623FC98A
P 3350 6880
AR Path="/61AA1D44/623FC98A" Ref="R?"  Part="1" 
AR Path="/62098590/623FC98A" Ref="R61"  Part="1" 
AR Path="/621C3274/623FC98A" Ref="R19"  Part="1" 
F 0 "R19" V 3400 6575 50  0000 C CNN
F 1 "10k" V 3345 6870 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 3350 6880 60  0001 C CNN
F 3 "" H 3350 6880 60  0000 C CNN
	1    3350 6880
	-1   0    0    1   
$EndComp
$Comp
L power1:+3.3V #PWR?
U 1 1 623FC99A
P 2785 6460
AR Path="/61AA1D44/623FC99A" Ref="#PWR?"  Part="1" 
AR Path="/62098590/623FC99A" Ref="#PWR035"  Part="1" 
AR Path="/621C3274/623FC99A" Ref="#PWR013"  Part="1" 
F 0 "#PWR013" H 2785 6310 50  0001 C CNN
F 1 "+3.3V" H 2680 6620 50  0000 C CNN
F 2 "" H 2785 6460 50  0001 C CNN
F 3 "" H 2785 6460 50  0001 C CNN
	1    2785 6460
	1    0    0    -1  
$EndComp
Wire Wire Line
	2785 6460 2785 6475
Wire Wire Line
	2785 6475 2870 6475
Wire Wire Line
	3270 6375 3350 6375
Wire Wire Line
	3350 6630 3350 6375
Connection ~ 3350 6375
Wire Wire Line
	3350 6375 3465 6375
Text Label 3465 6375 0    50   ~ 0
Water_Heat_Manual_OFF
$Comp
L Simbolos_Luis:SW_SPDT SW1
U 1 1 620D90DE
P 1455 1300
F 0 "SW1" H 1455 1585 50  0000 C CNN
F 1 "SW_SPDT" H 1455 1494 50  0000 C CNN
F 2 "Footprints_Luis:SW_100SP1T1B4M2QE" H 1455 1300 50  0001 C CNN
F 3 "1-1825137-6" H 1455 1300 50  0001 C CNN
	1    1455 1300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 620D90E4
P 1735 2165
AR Path="/61AA1D44/620D90E4" Ref="#PWR?"  Part="1" 
AR Path="/62098590/620D90E4" Ref="#PWR030"  Part="1" 
AR Path="/621C3274/620D90E4" Ref="#PWR08"  Part="1" 
F 0 "#PWR08" H 1735 2165 30  0001 C CNN
F 1 "GND" H 1735 2095 30  0001 C CNN
F 2 "" H 1735 2165 60  0000 C CNN
F 3 "" H 1735 2165 60  0000 C CNN
	1    1735 2165
	1    0    0    -1  
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 620D90EA
P 1735 1705
AR Path="/61AA1D44/620D90EA" Ref="R?"  Part="1" 
AR Path="/62098590/620D90EA" Ref="R48"  Part="1" 
AR Path="/621C3274/620D90EA" Ref="R10"  Part="1" 
F 0 "R10" V 1785 1400 50  0000 C CNN
F 1 "10k" V 1730 1695 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1735 1705 60  0001 C CNN
F 3 "" H 1735 1705 60  0000 C CNN
	1    1735 1705
	-1   0    0    1   
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 620D90F0
P 1830 1905
AR Path="/61AA1D44/620D90F0" Ref="R?"  Part="1" 
AR Path="/62098590/620D90F0" Ref="R50"  Part="1" 
AR Path="/621C3274/620D90F0" Ref="R13"  Part="1" 
F 0 "R13" V 1740 1805 50  0000 C CNN
F 1 "10k" V 1825 1890 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1830 1905 60  0001 C CNN
F 3 "" H 1830 1905 60  0000 C CNN
	1    1830 1905
	-1   0    0    1   
$EndComp
Wire Wire Line
	1735 1955 1735 2155
Wire Wire Line
	1830 2155 1735 2155
Connection ~ 1735 2155
Wire Wire Line
	1735 2155 1735 2165
$Comp
L power1:+3.3V #PWR?
U 1 1 620D90FA
P 1170 1285
AR Path="/61AA1D44/620D90FA" Ref="#PWR?"  Part="1" 
AR Path="/62098590/620D90FA" Ref="#PWR019"  Part="1" 
AR Path="/621C3274/620D90FA" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 1170 1135 50  0001 C CNN
F 1 "+3.3V" H 1065 1445 50  0000 C CNN
F 2 "" H 1170 1285 50  0001 C CNN
F 3 "" H 1170 1285 50  0001 C CNN
	1    1170 1285
	1    0    0    -1  
$EndComp
Wire Wire Line
	1170 1285 1170 1300
Wire Wire Line
	1170 1300 1255 1300
Wire Wire Line
	1655 1200 1735 1200
Wire Wire Line
	1655 1400 1830 1400
Wire Wire Line
	1735 1455 1735 1200
Connection ~ 1735 1200
Wire Wire Line
	1735 1200 1850 1200
Wire Wire Line
	1830 1655 1830 1400
Connection ~ 1830 1400
Wire Wire Line
	1830 1400 1860 1400
$Comp
L Simbolos_Luis:SW_SPDT SW2
U 1 1 620E167C
P 1505 2675
F 0 "SW2" H 1505 2960 50  0000 C CNN
F 1 "SW_SPDT" H 1505 2869 50  0000 C CNN
F 2 "Footprints_Luis:SW_100SP1T1B4M2QE" H 1505 2675 50  0001 C CNN
F 3 "1-1825137-6" H 1505 2675 50  0001 C CNN
	1    1505 2675
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 620E1682
P 1785 3540
AR Path="/61AA1D44/620E1682" Ref="#PWR?"  Part="1" 
AR Path="/62098590/620E1682" Ref="#PWR031"  Part="1" 
AR Path="/621C3274/620E1682" Ref="#PWR09"  Part="1" 
F 0 "#PWR09" H 1785 3540 30  0001 C CNN
F 1 "GND" H 1785 3470 30  0001 C CNN
F 2 "" H 1785 3540 60  0000 C CNN
F 3 "" H 1785 3540 60  0000 C CNN
	1    1785 3540
	1    0    0    -1  
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 620E1688
P 1785 3080
AR Path="/61AA1D44/620E1688" Ref="R?"  Part="1" 
AR Path="/62098590/620E1688" Ref="R49"  Part="1" 
AR Path="/621C3274/620E1688" Ref="R11"  Part="1" 
F 0 "R11" V 1835 2775 50  0000 C CNN
F 1 "10k" V 1780 3070 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1785 3080 60  0001 C CNN
F 3 "" H 1785 3080 60  0000 C CNN
	1    1785 3080
	-1   0    0    1   
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 620E168E
P 1880 3280
AR Path="/61AA1D44/620E168E" Ref="R?"  Part="1" 
AR Path="/62098590/620E168E" Ref="R52"  Part="1" 
AR Path="/621C3274/620E168E" Ref="R15"  Part="1" 
F 0 "R15" V 1790 3180 50  0000 C CNN
F 1 "10k" V 1875 3265 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1880 3280 60  0001 C CNN
F 3 "" H 1880 3280 60  0000 C CNN
	1    1880 3280
	-1   0    0    1   
$EndComp
Wire Wire Line
	1785 3330 1785 3530
Wire Wire Line
	1880 3530 1785 3530
Connection ~ 1785 3530
Wire Wire Line
	1785 3530 1785 3540
$Comp
L power1:+3.3V #PWR?
U 1 1 620E1698
P 1220 2660
AR Path="/61AA1D44/620E1698" Ref="#PWR?"  Part="1" 
AR Path="/62098590/620E1698" Ref="#PWR020"  Part="1" 
AR Path="/621C3274/620E1698" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 1220 2510 50  0001 C CNN
F 1 "+3.3V" H 1115 2820 50  0000 C CNN
F 2 "" H 1220 2660 50  0001 C CNN
F 3 "" H 1220 2660 50  0001 C CNN
	1    1220 2660
	1    0    0    -1  
$EndComp
Wire Wire Line
	1220 2660 1220 2675
Wire Wire Line
	1220 2675 1305 2675
Wire Wire Line
	1705 2575 1785 2575
Wire Wire Line
	1705 2775 1880 2775
Wire Wire Line
	1785 2830 1785 2575
Connection ~ 1785 2575
Wire Wire Line
	1785 2575 1900 2575
Wire Wire Line
	1880 3030 1880 2775
Connection ~ 1880 2775
Wire Wire Line
	1880 2775 1910 2775
$Comp
L Simbolos_Luis:SW_SPDT SW3
U 1 1 620EA980
P 1525 4030
F 0 "SW3" H 1525 4315 50  0000 C CNN
F 1 "SW_SPDT" H 1525 4224 50  0000 C CNN
F 2 "Footprints_Luis:SW_100SP1T1B4M2QE" H 1525 4030 50  0001 C CNN
F 3 "1-1825137-6" H 1525 4030 50  0001 C CNN
	1    1525 4030
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 620EA986
P 1805 4895
AR Path="/61AA1D44/620EA986" Ref="#PWR?"  Part="1" 
AR Path="/62098590/620EA986" Ref="#PWR032"  Part="1" 
AR Path="/621C3274/620EA986" Ref="#PWR010"  Part="1" 
F 0 "#PWR010" H 1805 4895 30  0001 C CNN
F 1 "GND" H 1805 4825 30  0001 C CNN
F 2 "" H 1805 4895 60  0000 C CNN
F 3 "" H 1805 4895 60  0000 C CNN
	1    1805 4895
	1    0    0    -1  
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 620EA98C
P 1805 4435
AR Path="/61AA1D44/620EA98C" Ref="R?"  Part="1" 
AR Path="/62098590/620EA98C" Ref="R54"  Part="1" 
AR Path="/621C3274/620EA98C" Ref="R12"  Part="1" 
F 0 "R12" V 1855 4130 50  0000 C CNN
F 1 "10k" V 1800 4425 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1805 4435 60  0001 C CNN
F 3 "" H 1805 4435 60  0000 C CNN
	1    1805 4435
	-1   0    0    1   
$EndComp
$Comp
L power1:+3.3V #PWR?
U 1 1 620EA99C
P 1240 4015
AR Path="/61AA1D44/620EA99C" Ref="#PWR?"  Part="1" 
AR Path="/62098590/620EA99C" Ref="#PWR026"  Part="1" 
AR Path="/621C3274/620EA99C" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 1240 3865 50  0001 C CNN
F 1 "+3.3V" H 1135 4175 50  0000 C CNN
F 2 "" H 1240 4015 50  0001 C CNN
F 3 "" H 1240 4015 50  0001 C CNN
	1    1240 4015
	1    0    0    -1  
$EndComp
Wire Wire Line
	1240 4015 1240 4030
Wire Wire Line
	1240 4030 1325 4030
Wire Wire Line
	1725 3930 1805 3930
Wire Wire Line
	1805 4185 1805 3930
Connection ~ 1805 3930
Wire Wire Line
	1805 3930 1920 3930
$Comp
L Simbolos_Luis:SW_SPDT SW4
U 1 1 620F4A6E
P 1550 5485
F 0 "SW4" H 1550 5770 50  0000 C CNN
F 1 "SW_SPDT" H 1550 5679 50  0000 C CNN
F 2 "Footprints_Luis:SW_100SP1T1B4M2QE" H 1550 5485 50  0001 C CNN
F 3 "1-1825137-6" H 1550 5485 50  0001 C CNN
	1    1550 5485
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 620F4A74
P 1830 6350
AR Path="/61AA1D44/620F4A74" Ref="#PWR?"  Part="1" 
AR Path="/62098590/620F4A74" Ref="#PWR033"  Part="1" 
AR Path="/621C3274/620F4A74" Ref="#PWR011"  Part="1" 
F 0 "#PWR011" H 1830 6350 30  0001 C CNN
F 1 "GND" H 1830 6280 30  0001 C CNN
F 2 "" H 1830 6350 60  0000 C CNN
F 3 "" H 1830 6350 60  0000 C CNN
	1    1830 6350
	1    0    0    -1  
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 620F4A7A
P 1830 5890
AR Path="/61AA1D44/620F4A7A" Ref="R?"  Part="1" 
AR Path="/62098590/620F4A7A" Ref="R55"  Part="1" 
AR Path="/621C3274/620F4A7A" Ref="R14"  Part="1" 
F 0 "R14" V 1880 5585 50  0000 C CNN
F 1 "10k" V 1825 5880 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1830 5890 60  0001 C CNN
F 3 "" H 1830 5890 60  0000 C CNN
	1    1830 5890
	-1   0    0    1   
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 620F4A80
P 1925 6090
AR Path="/61AA1D44/620F4A80" Ref="R?"  Part="1" 
AR Path="/62098590/620F4A80" Ref="R59"  Part="1" 
AR Path="/621C3274/620F4A80" Ref="R17"  Part="1" 
F 0 "R17" V 1835 5990 50  0000 C CNN
F 1 "10k" V 1920 6075 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1925 6090 60  0001 C CNN
F 3 "" H 1925 6090 60  0000 C CNN
	1    1925 6090
	-1   0    0    1   
$EndComp
Wire Wire Line
	1830 6140 1830 6340
Wire Wire Line
	1925 6340 1830 6340
Connection ~ 1830 6340
Wire Wire Line
	1830 6340 1830 6350
$Comp
L power1:+3.3V #PWR?
U 1 1 620F4A8A
P 1265 5470
AR Path="/61AA1D44/620F4A8A" Ref="#PWR?"  Part="1" 
AR Path="/62098590/620F4A8A" Ref="#PWR027"  Part="1" 
AR Path="/621C3274/620F4A8A" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 1265 5320 50  0001 C CNN
F 1 "+3.3V" H 1160 5630 50  0000 C CNN
F 2 "" H 1265 5470 50  0001 C CNN
F 3 "" H 1265 5470 50  0001 C CNN
	1    1265 5470
	1    0    0    -1  
$EndComp
Wire Wire Line
	1265 5470 1265 5485
Wire Wire Line
	1265 5485 1350 5485
Wire Wire Line
	1750 5385 1830 5385
Wire Wire Line
	1750 5585 1925 5585
Wire Wire Line
	1830 5640 1830 5385
Connection ~ 1830 5385
Wire Wire Line
	1830 5385 1945 5385
Wire Wire Line
	1925 5840 1925 5585
Connection ~ 1925 5585
Wire Wire Line
	1925 5585 1955 5585
$Comp
L Simbolos_Luis:SW_SPDT SW5
U 1 1 62100372
P 1600 6780
F 0 "SW5" H 1600 7065 50  0000 C CNN
F 1 "SW_SPDT" H 1600 6974 50  0000 C CNN
F 2 "Footprints_Luis:SW_100SP1T1B4M2QE" H 1600 6780 50  0001 C CNN
F 3 "1-1825137-6" H 1600 6780 50  0001 C CNN
	1    1600 6780
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62100378
P 1880 7645
AR Path="/61AA1D44/62100378" Ref="#PWR?"  Part="1" 
AR Path="/62098590/62100378" Ref="#PWR034"  Part="1" 
AR Path="/621C3274/62100378" Ref="#PWR012"  Part="1" 
F 0 "#PWR012" H 1880 7645 30  0001 C CNN
F 1 "GND" H 1880 7575 30  0001 C CNN
F 2 "" H 1880 7645 60  0000 C CNN
F 3 "" H 1880 7645 60  0000 C CNN
	1    1880 7645
	1    0    0    -1  
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 6210037E
P 1880 7185
AR Path="/61AA1D44/6210037E" Ref="R?"  Part="1" 
AR Path="/62098590/6210037E" Ref="R57"  Part="1" 
AR Path="/621C3274/6210037E" Ref="R16"  Part="1" 
F 0 "R16" V 1930 6880 50  0000 C CNN
F 1 "10k" V 1875 7175 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1880 7185 60  0001 C CNN
F 3 "" H 1880 7185 60  0000 C CNN
	1    1880 7185
	-1   0    0    1   
$EndComp
$Comp
L lsts-passives:R R?
U 1 1 62100384
P 1975 7385
AR Path="/61AA1D44/62100384" Ref="R?"  Part="1" 
AR Path="/62098590/62100384" Ref="R60"  Part="1" 
AR Path="/621C3274/62100384" Ref="R18"  Part="1" 
F 0 "R18" V 1885 7285 50  0000 C CNN
F 1 "10k" V 1970 7370 50  0000 C CNN
F 2 "lsts-passives:R_0603" H 1975 7385 60  0001 C CNN
F 3 "" H 1975 7385 60  0000 C CNN
	1    1975 7385
	-1   0    0    1   
$EndComp
Wire Wire Line
	1880 7435 1880 7635
Wire Wire Line
	1975 7635 1880 7635
Connection ~ 1880 7635
Wire Wire Line
	1880 7635 1880 7645
$Comp
L power1:+3.3V #PWR?
U 1 1 6210038E
P 1315 6765
AR Path="/61AA1D44/6210038E" Ref="#PWR?"  Part="1" 
AR Path="/62098590/6210038E" Ref="#PWR029"  Part="1" 
AR Path="/621C3274/6210038E" Ref="#PWR07"  Part="1" 
F 0 "#PWR07" H 1315 6615 50  0001 C CNN
F 1 "+3.3V" H 1210 6925 50  0000 C CNN
F 2 "" H 1315 6765 50  0001 C CNN
F 3 "" H 1315 6765 50  0001 C CNN
	1    1315 6765
	1    0    0    -1  
$EndComp
Wire Wire Line
	1315 6765 1315 6780
Wire Wire Line
	1315 6780 1400 6780
Wire Wire Line
	1800 6680 1880 6680
Wire Wire Line
	1800 6880 1975 6880
Wire Wire Line
	1880 6935 1880 6680
Connection ~ 1880 6680
Wire Wire Line
	1880 6680 1995 6680
Wire Wire Line
	1975 7135 1975 6880
Connection ~ 1975 6880
Wire Wire Line
	1975 6880 2005 6880
Text Label 1860 1400 0    50   ~ 0
All_Manual
Text Label 1900 2575 0    50   ~ 0
Tide_Manual_LOW
Text Label 1910 2775 0    50   ~ 0
Tide_Manual_HIGH
Wire Wire Line
	3270 6575 3475 6575
Wire Wire Line
	3350 7130 3350 7340
Wire Wire Line
	3290 5260 3495 5260
$Comp
L power:GND #PWR?
U 1 1 62101C04
P 3370 6025
AR Path="/61AA1D44/62101C04" Ref="#PWR?"  Part="1" 
AR Path="/62098590/62101C04" Ref="#PWR028"  Part="1" 
AR Path="/621C3274/62101C04" Ref="#PWR018"  Part="1" 
F 0 "#PWR018" H 3370 6025 30  0001 C CNN
F 1 "GND" H 3370 5955 30  0001 C CNN
F 2 "" H 3370 6025 60  0000 C CNN
F 3 "" H 3370 6025 60  0000 C CNN
	1    3370 6025
	1    0    0    -1  
$EndComp
Wire Wire Line
	3370 5815 3370 6025
Text Label 3755 1875 2    50   ~ 0
All_OFF
Text Label 3755 1775 2    50   ~ 0
All_Manual
NoConn ~ 4925 2075
NoConn ~ 4925 2175
NoConn ~ 4925 2275
NoConn ~ 4925 2375
NoConn ~ 4925 2475
NoConn ~ 3495 5260
NoConn ~ 3475 6575
Wire Wire Line
	1725 4130 1930 4130
NoConn ~ 1930 4130
Wire Wire Line
	1805 4685 1805 4895
$EndSCHEMATC
