// Configure an LS7366R quadrature counter and print its two mode registers once per second.
#include <Arduino.h>
#include <SPI.h>
#include "TeensySPI.h"

namespace {
struct ls7366r_options {
    uint8_t write_MDR0 = 0x88;
    uint8_t read_MDR0 = 0x48;
    uint8_t write_MDR1 = 0x90;
    uint8_t read_MDR1 = 0x50;
    uint8_t four_count = 0b11;
    uint8_t free_running = 0b00;
    uint8_t two_byte = 0b10;
};

const ls7366r_options ls7366r{};
int slave_select = 6;
constexpr int fcki_pin = 3;
TeensySPI spi0(&SPI);
SPISettings spi0_settings(300000, MSBFIRST, SPI_MODE1);

// Select a mode register and clock out its one-byte value.
uint8_t readMDR(uint8_t read_cmd) {
    uint8_t tx_bytes[1] = {read_cmd};
    uint8_t rx_bytes[1] = {};
    spi0.spiWriteThenRead(&slave_select, tx_bytes, 1, rx_bytes, 1);
    return rx_bytes[0];
}
}

// Start the counter's external clock and configure its quadrature and count modes.
void setup() {
    analogWriteResolution(8);
    analogWriteFrequency(fcki_pin, 3000000);
    analogWrite(fcki_pin, 128);  // Drive the counter clock at 50% duty cycle.

    pinMode(slave_select, OUTPUT);
    digitalWrite(slave_select, HIGH);
    SPI.begin();
    Serial.begin(115200);

    // MDR0 stores the count mode in bits 3:2 and the quadrature mode in bits 1:0.
    const uint8_t MDR0 = (ls7366r.free_running << 2) | ls7366r.four_count;
    const uint8_t MDR1 = ls7366r.two_byte;  // Bit 2 stays clear to enable counting.
    uint8_t tx_MDR0[2] = {ls7366r.write_MDR0, MDR0};
    uint8_t tx_MDR1[2] = {ls7366r.write_MDR1, MDR1};

    SPI.beginTransaction(spi0_settings);
    spi0.spiWrite(&slave_select, tx_MDR0, 2);
    spi0.spiWrite(&slave_select, tx_MDR1, 2);
    SPI.endTransaction();
}

// Read back the mode registers and print them as a tab-separated pair.
void loop() {
    SPI.beginTransaction(spi0_settings);
    const uint8_t MDR0 = readMDR(ls7366r.read_MDR0);
    const uint8_t MDR1 = readMDR(ls7366r.read_MDR1);
    SPI.endTransaction();

    Serial.print(MDR0);
    Serial.print('\t');
    Serial.println(MDR1);
    delay(1000);
}
