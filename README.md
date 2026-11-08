# edge-paladin
A C++17 industrial edge gateway built on ESP32-C3 and FreeRTOS.

## Overview
edge-paladin runs on a WifiDuino32-C3 board using ESP-IDF through PlatformIO.
Right now the firmware provides a UART command shell (PALADIN) for diagnosing
and interacting with the device, plus a status LED that blinks while the
firmware is running.

## Hardware
| Item        | Value                         |
|-------------|-------------------------------|
| Board       | WifiDuino32-C3 (ESP32-C3)     |
| Shell UART  | `UART_NUM_1`, 115200 baud     |
| UART TX/RX  | GPIO7 / GPIO6                 |
| Status LED  | GPIO8                         |

Datasheet and technical reference manual are in `docs/`.

## Project layout
```
src/main.c        Entry point: init UART, start shell task, blink status LED
lib/shell/        Command shell (line input, tokenizing, built-in commands)
lib/uart/         UART driver wrapper (init, read, write)
lib/network/      Wi-Fi station configuration (in progress)
docs/             ESP32-C3 datasheet and reference manual
```

## Build and flash
Requires [PlatformIO](https://platformio.org/).

1. Create a `config.env` in the project root (it is git-ignored) with the Wi-Fi credentials:
   ```ini
   [env]
   build_flags =
       -DWIFI_SSID=\"your-ssid\"
       -DWIFI_PASSWORD=\"your-password\"
   ```
2. Build, flash and open the monitor:
   ```sh
   pio run
   pio run -t upload -t monitor
   pio device monitor
   ```

Connect a serial terminal (115200 baud) to GPIO7/GPIO6 to use the shell.

## Shell
The shell runs in its own FreeRTOS task. It prints a banner on startup, then
reads a line over UART, splits it into arguments and runs the matching
built-in command. Built-ins are registered in the `builtInList` table in
`lib/shell/shell.c`.

## TODO

### 1. System diagnostics
These are probably the most useful commands to implement first.

| Done | Command   | Function                                    | ESP-IDF API / source        |
|------|-----------|---------------------------------------------|-----------------------------|
| [x]  | `help`    | List available commands                     | Your command registry       |
| [x]  | `clear`   | Clear terminal                              | ANSI escape sequences       |
| [x]  | `info`    | Chip model, cores, revision, flash size     | `esp_chip_info()`           |
| [x]  | `uptime`  | Time since boot                             | `esp_timer_get_time()`      |
| [ ]  | `heap`    | Free heap, minimum free heap, largest block | `heap_caps_get_*()`         |
| [ ]  | `tasks`   | List FreeRTOS tasks and their state         | `vTaskList()`               |
| [ ]  | `version` | Firmware version and build date             | Compile-time macros         |
| [x]  | `reset`   | Restart the ESP32                           | `esp_restart()`             |

### 2. Hardware interaction
Make the shell useful for interacting with the actual hardware.

| Done | Command                    | Function                         |
|------|----------------------------|----------------------------------|
| [ ]  | `gpio read <pin>`          | Read GPIO state                  |
| [ ]  | `gpio write <pin> <0/1>`   | Set GPIO output                  |
| [ ]  | `gpio mode <pin> <in/out>` | Configure GPIO direction         |
| [ ]  | `adc read <channel>`       | Read an ADC channel              |
| [ ]  | `pwm set <freq> <duty>`    | Configure PWM                    |
| [x]  | `uart status`              | Show UART configuration and status |
