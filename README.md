# ESP32 DHT22 and SPIFFS Project

This project demonstrates reading temperature and humidity from a DHT22 sensor using an ESP32, along with integrating a SPI Flash File System (SPIFFS) to store additional data. The complete firmware—including bootloader, partition table, main firmware, and SPIFFS image—is merged into a single binary for use in the Wokwi simulator.

## Table of Contents

- [Overview](#overview)
- [Project Setup](#project-setup)
- [Hardware Overview](#hardware-overview)
- [Software Configuration](#software-configuration)
  - [PlatformIO Configuration (.ini File Explanation)](#platformio-configuration-ini-file-explanation)
- [ESP32 Partition Table](#esp32-partition-table)
- [SPIFFS Filesystem](#spiffs-filesystem)
- [Building the Merged Firmware](#building-the-merged-firmware)
- [Wokwi Simulation and Running the Project](#wokwi-simulation-and-running-the-project)
- [Conclusion](#conclusion)

## Overview

In this project the ESP32 reads temperature and humidity values from a DHT22 sensor using the Adafruit DHT sensor library. In addition, a SPI Flash File System (SPIFFS) is integrated to store files (from the `/data` directory) in flash memory. To test the entire setup in the Wokwi simulator, a custom partition table is used and all binaries are merged into a single firmware file.

## Project Setup

- **Editor/IDE:** Visual Studio Code  
- **Build System:** PlatformIO  
- **Simulation Platform:** Wokwi  
- **MCU:** ESP32 (esp32dev board)  
- **Sensor:** DHT22  

The project uses the Arduino framework with the Adafruit DHT sensor library.

## Hardware Overview

- **ESP32:** The main microcontroller.
- **DHT22 Sensor:** Reads temperature and humidity, connected to an ESP32 GPIO.
- **SPI Flash:** Used for the SPIFFS filesystem to store extra files or configuration data.

## Software Configuration

### PlatformIO Configuration (.ini File Explanation)

The project configuration is managed by the `platformio.ini` file. This file tells PlatformIO how to build your project, which board and framework to use, and how to set up additional options. Here is the configuration used:

```ini
[env:esp32]
platform = espressif32
framework = arduino
board = esp32dev
board_build.partitions = partitions.csv
lib_deps = adafruit/DHT sensor library@^1.4.6
build_flags = 
    -D SPIFFS_USE_LITTLEFS
```

**Explanation of key lines:**

- **`platform = espressif32`**  
  Specifies the platform and toolchain for ESP32 devices.

- **`framework = arduino`**  
  Indicates that the Arduino framework is used for this project.

- **`board = esp32dev`**  
  Selects the ESP32 development board configuration.

- **`board_build.partitions = partitions.csv`**  
  Points to the custom partition table file (`partitions.csv`) that defines memory layout (nvs, factory, spiffs, coredump).

- **`lib_deps = adafruit/DHT sensor library@^1.4.6`**  
  Adds the Adafruit DHT sensor library as a dependency for sensor operations.

- **`build_flags = -D SPIFFS_USE_LITTLEFS`**  
  This flag defines `SPIFFS_USE_LITTLEFS` during compilation. It instructs the code to use the LittleFS library as a replacement for SPIFFS.  
  **Do you really need it?**  
  - If your project is designed to work with SPIFFS and you are not planning to switch to LittleFS, then you can remove this flag. However, some projects use LittleFS due to its improved performance and reliability compared to the older SPIFFS. Check your code and library documentation to decide which filesystem you intend to use.

- **Commented Lines:**  
  The lines:  
  ```ini
  ;upload_speed = 115200
  ;monitor_speed = 115200
  ```  
  are commented out (denoted by the leading `;`). These settings define:
  - **`upload_speed`**: The baud rate used when uploading the firmware to the device.
  - **`monitor_speed`**: The baud rate used for the serial monitor output.  
  Since they are commented out, the default settings are used. If you require a different speed for uploading or monitoring serial output, you can uncomment and adjust these values.

For further instructions and detailed information on creating and customizing a PlatformIO .ini file, refer to the [PlatformIO Project Configuration File documentation](https://docs.platformio.org/page/projectconf.html).

## ESP32 Partition Table

### Custom Partition Table (`partitions.csv`)

```csv
# Name, Type, SubType, Offset, Size
nvs,      data, nvs,     0x9000,  0x5000
factory,  app,  factory,  0x10000, 1M
spiffs,   data, spiffs,   0x110000, 0xE0000
coredump, data, coredump, 0x1F0000, 0x14000
```

**Partition Details:**

- **nvs:**  
  - **Type/SubType:** Data / nvs  
  - **Purpose:** Stores non-volatile settings and parameters.

- **factory:**  
  - **Type/SubType:** App / factory  
  - **Purpose:** Contains the main application firmware.

- **spiffs:**  
  - **Type/SubType:** Data / spiffs  
  - **Purpose:** Holds the SPI Flash File System. Files from the `/data` directory are stored here.  
  - **Offset & Size:** Starts at `0x110000` with a size of `0xE0000`.

- **coredump:**  
  - **Type/SubType:** Data / coredump  
  - **Purpose:** Reserved for storing crash dump information.

## SPIFFS Filesystem

**SPIFFS** stands for **SPI Flash File System**. "SPI" refers to the Serial Peripheral Interface used to communicate with flash memory devices, while "FFS" stands for Flash File System. SPIFFS is designed for microcontrollers with limited resources to efficiently store and manage files on SPI NOR flash.

- **Purpose:**  
  - Store files (such as configuration files, web assets, logs) in flash memory.
- **Usage in this Project:**  
  - The `/data` directory in your project contains files that are built into a SPIFFS image (`spiffs.bin`), which is later merged with the firmware.

## Building the Merged Firmware

Before merging, make sure to install esptool if you haven't already:

```bash
pip install esptool
```

To merge the binaries (bootloader, partition table, main firmware, and SPIFFS image) into one firmware file, run the following command from the `.pio/build/esp32` directory:

```bash
python -m esptool --chip esp32 merge_bin --output merged_firmware.bin --flash_mode dio --flash_size 4MB 0x1000 bootloader.bin 0x8000 partitions.bin 0x10000 firmware.bin 0x110000 spiffs.bin
```

### Breakdown of the Merge Command

- **`python -m esptool`**  
  Runs the `esptool` module as a script.

- **`--chip esp32`**  
  Specifies that the target chip is the ESP32.

- **`merge_bin`**  
  Tells esptool to merge several binary files into one.

- **`--output merged_firmware.bin`**  
  Sets the name of the final merged firmware file.

- **`--flash_mode dio`**  
  Configures the flash mode to DIO (Dual I/O), which affects how data is read from the flash memory.

- **`--flash_size 4MB`**  
  Specifies the total size of the flash memory.

- **`0x1000 bootloader.bin`**  
  Places the bootloader binary at offset `0x1000`.

- **`0x8000 partitions.bin`**  
  Places the partition table binary at offset `0x8000`.

- **`0x10000 firmware.bin`**  
  Places the main application firmware at offset `0x10000`.

- **`0x110000 spiffs.bin`**  
  Places the SPIFFS filesystem image at offset `0x110000`, matching the partition table configuration.

## Wokwi Simulation and Running the Project

### Wokwi Configuration (`wokwi.toml`)

Ensure your `wokwi.toml` file is set as follows:

```toml
[wokwi]
version = 1
elf = ".pio/build/esp32/firmware.elf"
firmware = ".pio/build/esp32/merged_firmware.bin"
```

### How to Run

1. **Build the Project:**  
   From your project directory, build the firmware with PlatformIO:
   ```bash
   pio run
   ```

2. **Build the SPIFFS Image:**  
   Ensure your `/data` directory contains the files to be stored, then build the filesystem:
   ```bash
   pio run --target buildfs
   ```

3. **Merge the Binaries:**  
   Run the esptool merge command (as described above) in the `.pio/build/esp32` directory.

4. **Modify the Wokwi Configuration:**  
   Ensure your `wokwi.toml` file is updated as shown above so that Wokwi knows where to find the ELF and merged firmware files.

5. **Run the Simulation:**  
   Open the `diagram.json` file in Visual Studio Code. With the Wokwi extension installed, simply press **Start** to launch the simulation.

## Conclusion

This README outlines:
- The setup of an ESP32 project using PlatformIO and reading sensor data from a DHT22.
- Configuring a custom partition table that allocates space for the application, SPIFFS filesystem, and other data.
- Using SPIFFS (SPI Flash File System) to store files.
- Details about the PlatformIO configuration (.ini file) including optional build flags and serial speeds.
- Installing esptool and merging all binaries into a single firmware image.
- Configuring and running the project in the Wokwi simulator.

For more details on PlatformIO configuration files, refer to the [PlatformIO Project Configuration documentation](https://docs.platformio.org/page/projectconf.html).