# Programmable Microcontroller-Based Signal Generator

## Overview
This project is a **programmable signal generator** based on the **ATmega328P** microcontroller and **AD9833 waveform generator IC**. It features waveform configuration via SPI communication and an interactive user interface using an LCD and push buttons.

## Team Members

| Name                | Roll No   | Section |
|---------------------|-----------|---------|
| Syed Affan Hussain  | EE-21156  | D       |
| Wajih Uddin         | EE-21153  | D       |
| Syed Alyan Ali      | EE-21158  | D       |
| Muhammad Arshad Khan| EE-21169  | D       |

## Project Summary
- Interfaced ATmega328P with AD9833 using SPI (Mode 2).
- Configured control, frequency, and phase registers of AD9833.
- Implemented a real-time user interface via an LCD to display waveform type and frequency.
- Push buttons used to adjust frequency and switch between sine, square, and triangle waveforms.
- Embedded C code written to manage communication, display, and user input.

## Hardware Details

### AD9833 Connections
- Operates at 2.3V to 5.5V.
- Interfaced using SPI Mode 2.
- Three-wire interface:
  - **SCLK** – Serial Clock
  - **SDATA** – Serial Data (MOSI)
  - **FSYNC** – Slave Select

### LCD Display
- Connected to display frequency (Hz/kHz) and waveform type.
- Managed via `lcd_command()` and `lcd_print()` functions.

## AD9833 Register Summary

### Control Register Bits
- `D15:D14` – Register Select (00 = control, 01/10 = frequency, 11 = phase)
- `D13 (B28)` – 28-bit frequency mode
- `D8` – Reset bit
- `D5, D3, D1` – Waveform control bits
- Other bits unused

### Frequency Formula
`FREQREG = (f_OUT × 2^28) / f_CLK`

### Register Selection
- `FSELECT (D11)` – Select FREQ0 or FREQ1
- `PSELECT (D12)` – Select PHASE0 or PHASE1

## Embedded C Code Overview

### Key Functions
- `AD_write16(uint16_t data)`
- `AD_setFrequency(float freq)`
- `AD_setPhase(uint16_t phase)`
- `AD_setMode(mode_t mode)` – mode can be `MODE_SINE`, `MODE_SQUARE`, `MODE_TRIANGLE`
- `AD_init()` – Initializes AD9833 and SPI
- `lcd_init()`, `lcd_command()`, `lcd_data()`, `lcd_print()`

### User Interaction
- **PC5** – Increase frequency by 100Hz
- **PC4** – Decrease frequency by 100Hz
- **PC3** – Cycle waveform type (triangle → sine → square)

## Results
- Accurate and adjustable waveform generation.
- Smooth integration of microcontroller and waveform generator.
- Responsive real-time interface using push buttons and LCD.


## References

- Analog Devices. (2019). *Low Power, 12.65 mW, 2.3 V to 5.5 V, Programmable Waveform Generator*. AD9833 Datasheet.
- Analog Devices. (2003). *AN-1070 Application Note: AD9833 Programming*.
- Atmel. (2015). *8-bit AVR Microcontroller with 32K Bytes In-System Programmable Flash*. ATmega328P Datasheet.
- Ioan, M. (2010). *Design of a Function Generator using Direct Digital Synthesis (DDS) Technology and PIC16F877A Microcontroller*. Annals of the Oradea University, 19(2).
- Rego, P. V. (2012). *Integrating 8-bit Micro-controllers in Ada*. Ada User Journal, 3(4).
- Rospawan, A., Simatupang, J. W., & Purnama, I. (2019). *A Simple, Cheap and Precise Microcontroller-Based DDS Function Generator*. Journal of Electrical and Electronics Engineering, 3(2), 118-121.
- Yashas, S. R., Kulkarni, S., & Kumara, B. (2019). *Waveform Generation using Direct Digital Synthesis (DDS) Technique*. International Research Journal of Engineering and Technology (IRJET), 6(11).
- Ionescu, C. (2020). *AVR-1000b: Getting Started with Writing C-Code for AVR MCUs*. Microchip Technology, Datasheet Module.
- Ada, W. F. (2012). *Low Distortion Signal Generator Based on Direct Digital Synthesis of ADC Characterization*. Acta IMEKO, 1(1), 59-64.



