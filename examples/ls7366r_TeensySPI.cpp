// Configure an LS7366R quadrature counter and print its two mode registers once per second.
#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>
#include "TeensySPI.h"

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

template <typename spi_ptr>
class LS7366R {
public:
    // Bind the SPI transport and its chip-select descriptor.
    LS7366R(spi_ptr p_spi_, int* slave_select_)
        : p_spi(p_spi_), slave_select(slave_select_) {}

    // Set the quadrature mode and counting mode.
    void writeMDR0(uint8_t quad_counts, uint8_t count_mode) {
        // Bits 3:2 contain the count mode and bits 1:0 contain the quadrature mode.
        const uint8_t MDR0 = (count_mode << 2) | quad_counts;
        uint8_t tx_bytes[2] = {ls7366r.write_MDR0, MDR0};
        p_spi->spiWrite(slave_select, tx_bytes, 2);
    }

    // Set the counter width and whether counting is enabled.
    void writeMDR1(uint8_t CNTR_len, bool counting_en) {
        const uint8_t MDR1 = CNTR_len | (counting_en ? 0x00 : 0x04);  // Bit 2 disables counting.
        uint8_t tx_bytes[2] = {ls7366r.write_MDR1, MDR1};
        p_spi->spiWrite(slave_select, tx_bytes, 2);
    }

    // Read the quadrature and counting mode register.
    uint8_t readMDR0() {
        return readMDR(ls7366r.read_MDR0);
    }

    // Read the counter width and enable register.
    uint8_t readMDR1() {
        return readMDR(ls7366r.read_MDR1);
    }

private:
    spi_ptr p_spi;
    int* slave_select;

    // Send a read opcode and clock out the register byte under the same chip select.
    uint8_t readMDR(uint8_t read_cmd) {
        uint8_t tx_bytes[1] = {read_cmd};
        uint8_t rx_bytes[1] = {};
        p_spi->spiWriteThenRead(slave_select, tx_bytes, 1, rx_bytes, 1);
        return rx_bytes[0];
    }
};

// Select the SPI backend here. The IC driver receives a pointer to this bus.
// The Teensy chip-select descriptor contains one GPIO pin number.
int slave_select[] = {6};
constexpr uint32_t spi_clock_hz = 300000;
SPISettings spi0_settings(spi_clock_hz, MSBFIRST, SPI_MODE1);

using SpiBus = TeensySPI;
SpiBus spi0(&SPI);

LS7366R<SpiBus*> counter(&spi0, slave_select);

// Start the bus and external counter clock, then configure the LS7366R.
void setup() {
    // Generate the LS7366R external clock on pin 3.
    constexpr int fcki_pin = 3;
    analogWriteResolution(8);
    analogWriteFrequency(fcki_pin, 3000000);
    analogWrite(fcki_pin, 128);  // 50% duty cycle.

    pinMode(slave_select[0], OUTPUT);
    digitalWrite(slave_select[0], HIGH);
    SPI.begin();
    Serial.begin(115200);

    SPI.beginTransaction(spi0_settings);
    counter.writeMDR0(ls7366r.four_count, ls7366r.free_running);
    counter.writeMDR1(ls7366r.two_byte, true);
    SPI.endTransaction();
}

// Print the two mode registers as a tab-separated pair.
void loop() {
    SPI.beginTransaction(spi0_settings);
    const uint8_t mdr0 = counter.readMDR0();
    const uint8_t mdr1 = counter.readMDR1();
    SPI.endTransaction();

    Serial.print(mdr0);
    Serial.print('\t');
    Serial.println(mdr1);
    delay(1000);
}
