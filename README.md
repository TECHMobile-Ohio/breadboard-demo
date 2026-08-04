# pico-breadboard-demo

This directory contains the firmware for a [TECHMobile](https://www.techmobileohio.org/) educational demo project using a breadboard and components from an [Elegoo 37 Sensor Kit](https://www.amazon.com/ELEGOO-Upgraded-Tutorial-Compatible-MEGA2560/dp/B01MG49ZQ5).

The project supports the following microcontroller targets:

| Target | Typical PlatformIO environment | Upload method |
|---|---|---|
| Raspberry Pi Pico | The Pico environment defined in `platformio.ini` | PlatformIO upload or manual `.uf2` copy |
| ESP32 development board | Commonly `esp32dev`, or the ESP32 environment defined in `platformio.ini` | PlatformIO serial upload |

See [the Google document](https://docs.google.com/document/d/1g_6uoyfyK8KvBRkyh1Naovmq_lL-QZgJ0V9mrtoPw3k/) for more detailed information and instructions on conducting the demo.

The firmware may already be installed on the demo device. The student activity involves connecting different combinations of sensors and output devices.

This project is managed and built with [PlatformIO](https://platformio.org/), which is available as an extension for [Visual Studio Code](https://code.visualstudio.com/). PlatformIO provides access to the Arduino framework's collection of hobbyist libraries and drivers while also managing project dependencies and package versions.

## Requirements

Before opening the project, make sure you have:

- A Windows, macOS, or Linux computer
- [Visual Studio Code](https://code.visualstudio.com/Download)
- The [PlatformIO IDE extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
- A USB **data** cable compatible with the selected board
- A Raspberry Pi Pico or supported ESP32 development board
- The complete project folder, including `platformio.ini`, `src`, and any required project libraries

PlatformIO Core does not need to be installed separately when the PlatformIO IDE extension is used in Visual Studio Code.

## IDE setup

### Install PlatformIO in Visual Studio Code

1. Install and open Visual Studio Code.
2. Open the **Extensions** view by selecting the Extensions icon in the left sidebar.
3. Search for **PlatformIO IDE**.
4. Select the extension published by **PlatformIO** and click **Install**.
5. Allow Visual Studio Code to reload if prompted.

The extension can also be installed from a terminal:

```bash
code --install-extension platformio.platformio-ide
```

### Open the project

1. In Visual Studio Code, select **File > Open Folder**.
2. Select the complete project folder—not only the `src` folder.
3. Confirm that `platformio.ini` is visible in the Explorer panel.
4. Wait for PlatformIO to finish loading the project and its environments.
5. If Visual Studio Code displays a Workspace Trust prompt, trust the folder only when it came from a source you recognize.

> **Important:** The folder containing `platformio.ini` must be opened as the project root. PlatformIO uses this file to determine which board environments and upload settings are available.

## Flashing a device

Connect only the board you intend to program, then use the instructions for that target.

### Select the correct PlatformIO environment

If the project contains more than one environment, expand **PlatformIO > Project Tasks** and select the environment for the connected board.

You can also specify an environment from the PlatformIO terminal:

```bash
pio run -e ENVIRONMENT_NAME -t upload
```

Replace `ENVIRONMENT_NAME` with the matching environment name from `platformio.ini`.

---

## Raspberry Pi Pico

### Enter bootloader mode

You can put the Pico into USB bootloader mode by:

- Holding the `BOOTSEL` button while connecting the USB cable, or
- Using the project's supported reset method, when available

When the Pico enters bootloader mode, it appears as a removable drive named `RPI-RP2`.

### Upload with PlatformIO

1. Open **PlatformIO > Project Tasks**.
2. Expand the Pico environment.
3. Expand **General**.
4. Select **Upload**.
5. Wait for the terminal to report a successful upload.

The PlatformIO Upload action may be able to reset the Pico automatically. When automatic reset does not work, place the board in bootloader mode manually before selecting **Upload**.

### Program manually with a firmware file

1. Put the Pico into bootloader mode.
2. Locate the generated `firmware.uf2` file in the Pico build directory.
3. Drag `firmware.uf2` onto the `RPI-RP2` removable drive.
4. The Pico copies the file, disconnects from bootloader mode, and restarts with the new firmware.

---

## ESP32

### Connect the board

1. Connect the ESP32 development board with a USB data cable.
2. Wait for the operating system to detect its serial port.
3. Close any serial monitor or other application already using that port.

Most ESP32 development boards can be placed into download mode automatically by the upload tool.

### Upload with PlatformIO

1. Open **PlatformIO > Project Tasks**.
2. Expand the ESP32 environment, commonly named `esp32dev`.
3. Expand **General**.
4. Select **Upload**.
5. Watch the PlatformIO terminal while the firmware is written.
6. Confirm that the terminal ends with `[SUCCESS]`.

The equivalent terminal command is:

```bash
pio run -e esp32dev -t upload
```

If the project uses a different ESP32 environment name, replace `esp32dev` with the name defined in `platformio.ini`.

### Enter ESP32 download mode manually

Use this procedure when the upload repeatedly stops at `Connecting...` or reports that it could not connect to the ESP32:

1. Start the PlatformIO **Upload** task.
2. When the terminal displays `Connecting...`, press and hold the board's **BOOT** button.
3. Release **BOOT** after the upload begins.
4. If the board does not restart automatically after the upload, press **EN** or **RESET** once.

On ESP32 hardware without an automatic USB-to-serial reset circuit, the serial bootloader is entered by holding GPIO0 low while the board resets. Development boards normally expose this function through the **BOOT** button.

### Check the serial port

To list detected serial devices, open a PlatformIO terminal and run:

```bash
pio device list
```

To upload through a specific port:

```bash
pio run -e esp32dev -t upload --upload-port PORT_NAME
```

Examples of port names include `COM5` on Windows, `/dev/cu.usbserial-0001` on macOS, and `/dev/ttyUSB0` on Linux.

> **Note:** Unlike the Pico's USB bootloader workflow, a typical ESP32 is flashed through its serial bootloader. Do not copy `firmware.bin` to a removable drive. Use PlatformIO's **Upload** task so all required ESP32 firmware images are written to the correct flash addresses.

---

## Serial monitor

After programming either board, open the PlatformIO serial monitor to view diagnostic output:

```bash
pio device monitor
```

If the project defines a monitor speed in `platformio.ini`, PlatformIO uses that value. Otherwise, the monitor speed must match the baud rate configured by the firmware.

## Troubleshooting

### No device or serial port appears

- Use a known USB data cable; some cables provide power only.
- Try a different USB port.
- Connect the board directly instead of through an unpowered hub.
- Reconnect the board and run `pio device list`.
- Install the USB-to-serial driver required by the specific ESP32 board, when applicable.

### The serial port is busy

Close any application using the board, including:

- PlatformIO Serial Monitor
- Arduino IDE Serial Monitor
- Another Visual Studio Code window
- A terminal program such as PuTTY, `screen`, or CoolTerm

### Uploading to the wrong board

Confirm that the active PlatformIO environment matches the connected board. When using the terminal, specify the environment explicitly:

```bash
pio run -e ENVIRONMENT_NAME -t upload
```

### ESP32 upload fails partway through

Try a direct USB connection and a shorter cable. If necessary, reduce the ESP32 upload speed in the matching environment in `platformio.ini`:

```ini
upload_speed = 115200
```

## Additional documentation

- [PlatformIO IDE for Visual Studio Code](https://docs.platformio.org/en/stable/integration/ide/vscode.html)
- [PlatformIO Espressif 32 platform](https://docs.platformio.org/en/stable/platforms/espressif32.html)
- [Espressif ESP32 boot-mode selection](https://docs.espressif.com/projects/esptool/en/latest/esp32/advanced-topics/boot-mode-selection.html)
