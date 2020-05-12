# ESP32-GATEWAY
ESP32 WIFI / BLE development board with Ethernet, micro SD card, GPIOs made with KiCAD.

Notice that we use the latest nightly KiCad builds.

Hardware change log:

- Hardware revision G

1. R54 removed. R24 and R16 moved further away from the button.
2. Improved EN signal - added C5(1uF/0603) on pin #3 of ESP32. R24 now 10k.

- Hardware revision F

>Added 3D view. Major changes to GPIO header, SD card mode, power consumption!:

1. GPIO6-11, which are used by the ESP32's internal flash, were disconnected from CON1 
and the 6 SD card signals are now routed instead for use when SD card is not present;
2.  SD card changed from 4-bit mode of operation to 1-bit mode of operation! Added 3 
NA resistor pads (R9-R11) for backwards compatiblity.
3. A lot of deep sleep mode improvements to lower the consumption to 100uA or less:

>R19 was changed from 4.99k/1%/R0603 to 220k/R0603;

>R20 was changed from 1.1k/1%/R0603 to 49.9k/1%/R0603;

>R30 was changed from 10k/0603 to 1M/R0603;

>LED_E1 jumper added to provide an option to disable the power LED;

>R24 changed from 1k/0603 to 1M/R0603;

>U5(CH340)'s pin 20disconnected from GND and left opened;

>CH340 now powered only when USB is present;

>R35 and R40 were changed from 220R/R0603 to 10k/R0603;

>D1 and D3 added, 1N5819S4/SOD123, as level shifters;

4. L2 was changed from 2.2uH/1.5A/DCR<0.1R/CD32 to 2.2uH/1.5A/DCR=72mR/20%/3.00x3.00x1.50mm/CD32(NR3015T2R2M);
5. U3's value was changed from ESP-WROOM-32 to ESP32-WROOM-32D-4MB;
6. U9(BAT54C(SOT23-3)) was renamed to D4;

- Hardware revision E1

1. Added C2/22uF/0603 for slower PHY powering

- Hardware revision E

1. R5 connected after T1's collector and value changed from NA to 1K. 
2. Improved drill distances.

- Hardware revision D

>Major Ethernet schematic changes, now similar to ESP32-PoE:

1. LAN8710's pin XTAL1 was disconnected from GPIO0 and connected to GPIO17/EMAC_CLK_OUT_180. GPIO17 
is now source clock. GPIO5 is now used as Ethernet "power enable pin".
2. CR1(Q50MHz/25ppm/3V/4P/5x3.2mm) and all surrounding elements were deleted.
3. U8(NCP303LSN27T1G(SOT-23-5)) and all surrounding elements were deleted.
4. U10(BAT54C), R44 and R5 were deleted.
5. FET1, R5, R6, C2, T1 and L1 were added to enable Ethernet power on/off feature.
6. GPIO5 is used for Ethernet power on/off.

- Hardware revision C

1. R5(1k) resistor added between OSC_EN and NRST(pin #19) of LAN8710A chip. Later, it was changed to NA!
2. U2 changed from SY8009AAAC(SOT23-5) to SY8089AAAC(SOT23-5).
3. FET2 changed from IRLML6402 to WPM2015-3/TR.
4. ESP32-WROOM module pad improvements 

>Major changes to the Ethernet PHY and reset similar to EVB's_Rev_D1:

5. U8 changed from BD5230G-TR(SSOP5/SOT-23-5) to NCP303LSN27T1G(SOT-23-5).
6. C28 changed from 10nF/50V/20%/Y5V/C0603 to 470nF/16V/10%/X7R/C0603.
7. C18 changed from 10uF/6.3V/20%/X5R/C0603 to NA.
8. R38 changed from 100k to 10k/R0603.
9. R5 changed from 1k/R0603 to NA.
