# pico-breadboard-demo

This directory contains the firmware for a [TECHMobile](https://www.techmobileohio.org/) educational demo project using a Raspberry Pi Pico, a breadboard, and components from an [Elegoo 37 Sensor Kit](https://www.amazon.com/ELEGOO-Upgraded-Tutorial-Compatible-MEGA2560/dp/B01MG49ZQ5).

See [the google doc](https://docs.google.com/document/d/1g_6uoyfyK8KvBRkyh1Naovmq_lL-QZgJ0V9mrtoPw3k/) for more detailed information and instructions on conducting the demo.
The firmware is pre-installed on the device, and the student activity involves hooking up a few different combinations of sensors and output devices.

This project is managed and built with [PlatformIO](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide), which is available as an extension for [VSCode](https://code.visualstudio.com/). PlatformIO was used because it has access to the large colleciton of hobbyist libraries and drivers in the Arduino framework, but provides a better way to manage packages and versions.

## IDE Setup

Open a VSCode workspacde in this directory and install the PlatformIO IDE extension, either from the suggested list or via CLI:

```bash
code --install-extension platformio.platformio-ide
```

## Flashing a Device

### Entering Bootloader Mode

You can put the Pico into bootloader mode to receive new firmware either by:

- holding the `BOOTSEL` button while plugging in the USB cable
- or double-tapping the `RUN` pin to ground to reset the device 2x within 200ms

In either case, the green LED should flash momentarily and then the device should show up as a removable drive with 2 files.

### Program via PlatformIO

The PlatformIO "Upload" button on the toolbar should allow you to flash the firmware onto the raspberry pi pico, but the method it uses to put the device into bootloader mode may not work, so you might have to first manually do one of the methods above.

### Program Manually with a firmware file

Manually put the device into bootloader mode, then drag and drop the `firmware.uf2` file into the removable drive that shows up. It should upload the file, then the device should flash itself and reboot into the new firmware.
