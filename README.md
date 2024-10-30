# STM32 WS2815 LED Controller

This is a hobby project using an STM32 microcontroller to control WS2815 LED strips over ethernet. While it’s a ~fun~ interesting setup, it’s still a work in progress, so the design and code have plenty of room for improvement.

## Hardware Overview

The controller is built around a custom PCB and features:

- **Microcontroller**: STM32G0B0RE, which handles LED effects, strip control, and MQTT connection.
- **Ethernet**: W5500 chip (from Wiznet) for simple 100/10 ethernet functionality.
- **LED Connection**: 4-pin connector for attaching WS2815 LED strips.
- **Debugging Options**:
  - **SWD (Serial Wire Debug)**: Required for flashing firmware.
  - **USB-C Port**: Used mainly for serial output, with optional 5V power.
- **Power Setup**:
  - **12V Power Connector**: Powers the LED strip, switched on/off with a software-controlled load switch. The power connector is subject to change in a future board REV but for now the power leads are attached using M4 machine screws.
  - **Buck Converter**: Steps down 12V to 5V for the data signals to the LEDs and to power the STM32 and ethernet chip.

*Known Issues*: 
  - The pads' hole size for the 4-pin LED strip connector is too small to easily fit the wires.

## Firmware Details

The firmware is written in C++ with portability and extensibility as guiding principles:

- **Connectivity**: Includes partially complete, platform-independent abstractions for network interface, sockets, and MQTT client, allowing connection to MQTT servers. Implementations relies on open-source libraries but the design allows these libraries to be swapped out relatively easily.
- **LED Strip Compatibility**: The firmware’s design allows control beyond the WS2815, with flexibility for WS2812, WS2811, and potentially other types through a semi-extensible interface. Currently, communication is handled by DMA timers and a circular buffer, but this setup supports future implementations.
- **Configurable, Extensible Architecture**: Aiming for maximum configurability, the design focuses on semi-modular components that can be swapped or extended easily.
- **Documentation**: Classes are documented using Doxygen. You can find a compiled HTML version in the `Firmware/Docs` folder.

*Known Issues*: 
  - The MQTT connection intermittently drops despite active ethernet indicators (likely a bug in the reconnect code).

---

## Getting Started

To try it out:

1. **Connect Power and LEDs**:
   - Screw down a 12V power source onto the marked.
   - Attach a WS2815 LED strip to the 4-pin connector.
   
2. **Upload Firmware**:
   - Firmware currently needs to be flashed over the SWD connector.

3. **Run and Debug**:
   - Connect the USB-C port for serial output or use SWD for debugging.

## Specs

| Feature                   | Description                                       |
|---------------------------|---------------------------------------------------|
| Microcontroller           | STM32G0B0RE                                       |
| Ethernet                  | W5500 (100/10 Ethernet)                           |
| LED Output Connector      | 4-pin (for WS2815 LED strip)                      |
| Debugging                 | USB-C, SWD for flashing      |
| Power Input               | 12V screw terminal                                |
| Voltage Conversion        | Buck converter (12V to 5V)                        |
| Data Buffer               | SN74125N for 5V data signals                      |
| Regulator                 | 3.3V for STM32 and W5500                          |

---

### Notes

This project is more of a learning exercise in PCB design and STM32 programming, so it might not be the most optimized controller out there! Any feedback or suggestions are welcome.
