# BLE HID Keyboard
A project to convert a [Perixx PERIDUO-307B USB wired keyboard](https://www.amazon.com/Perixx-PERIBOARD-407B-Mini-Keyboard-Dimension/dp/B0053O9ZNQ/) to wireless Bluetooth Low Energy (BLE). The conversion is accomplished by replacing the original PCB with the one documented here which uses a [Nordic nRF51422](https://www.nordicsemi.com/eng/Products/ANT/nRF51422) BLE capable ARM Cortex-M0 microcontroller. This repository includes the Eagle CAD PCB design files and firmware source code.

## Bill of Materials
Description | Quantity | Part Number(s) 
------------|----------|---------------
[1.2kΩ Resistor - 0805](https://www.adafruit.com/product/441) | 3 | R1, R2, R3
[Microchip MCP23018 I2C I/O Expander](https://www.digikey.com/product-detail/en/microchip-technology/MCP23018-E-SS/MCP23018-E-SS-ND/1999506) | 2 | IC1, IC2
[Samtec 2x5 SMT Micro Header](https://www.digikey.com/product-detail/en/FTSH-105-01-L-DV-K/SAM8799-ND/1875039) | 1 | JP1
[Dynastream N5150M5CD - nRF51422 Module](https://www.digikey.com/products/en?keywords=N5150M5CD) | 1 | U1
[Green SMT LED](https://www.digikey.com/product-detail/en/kingbright/APTD3216LCGCK/754-2049-1-ND/7043101) | 3 | LED_CAPS, LED_CONNECT, LED_NUM LED

It also requires a 2x13 ribbon cable connector harvested from the original PCB using a [Chip Quick SMD Removal Kit](https://www.adafruit.com/product/2660).

## Firmware Build
I have only tested the build on macOS but it will probably work on other Posix operating systems too.

**Note:** Building the firmware and deploying it to the nRF51 requires the **mergehex** and **nrfjprog** tools. The archive for your OS can be downloaded from the _Downloads_ tab of the [nRF51422 product page](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF51422). You will also need a SEGGER J-Link JTAG/SWD Debugger. If your projects are of a non-commercial nature, you can use this [low cost module available from Adafruit](https://www.adafruit.com/product/3571).

* ```make sdk``` will download and install the required Nordic SDK. This should only need to be done once.
* ```make flash_softdevice``` will install the required Nordic SoftDevice on the nRF51422 microcontroller using the J-Link debugger. This will typically only need to be done once for each microcontroller.
* ```make all``` will build the keyboard firmware.
* ```make flash```will build the keyboard firmware and deploy it using the J-Link debugger.
* ```make generate_patches``` can be used to generate diffs for any files that you have modified in the [config/ subfolder](https://github.com/adamgreen/BleHidKeyboard/tree/master/firmware/config) so that you can commit and share them via git.
