# TeensySPI

TeensySPI wraps a Teensy `SPIClass` bus with chip-select handling and helpers for register reads, writes, and full-duplex transfers. IC drivers can use the wrapper as their SPI transport.

## Install

Add the `src/` directory to your Teensy project and make `TeensySPI.h` available on the include path. The library uses the Arduino and SPI APIs bundled with the Teensy board core.

## Usage

Start the SPI bus with `SPI.begin()`. Configure the chip-select pin as an output and drive it high. Create a `TeensySPI` object with `&SPI`, then pass it to your device driver or call its transfer methods directly. Use `beginTransaction()` and `endTransaction()` on the underlying bus to apply the peripheral's clock rate, bit order, and SPI mode.

## Examples

Build each example as a separate Teensy sketch and open the serial monitor at 115200 baud. Both use the board's default SPI pins. Each example defines its own IC commands and uses TeensySPI directly.

- [LS7366R mode register readback](examples/ls7366r_TeensySPI.cpp) — Configures the counter and prints its mode registers. Connect chip select to pin 6 and the counter clock input to pin 3.
- [MPL115A1 raw pressure read](examples/mpl115a1_TeensySPI.cpp) — Prints the raw pressure ADC value. Connect chip select to pin 2 and hold the sensor's shutdown input high.
