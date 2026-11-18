// Read an MPL115A1 raw pressure ADC value over SPI and print it once per second.
#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>
#include "TeensySPI.h"

template <typename spiPtr>
class MPL115A1 {
public:
    // Bind the SPI transport, chip-select descriptor, and board delay function.
    MPL115A1(spiPtr p_spi_, int* p_slave_select_, void (*delay_ms_)(uint32_t))
        : p_spi(p_spi_), p_slave_select(p_slave_select_), delay_ms(delay_ms_) {}

    // Start a conversion, then read and combine the two pressure ADC register bytes.
    uint16_t readPadc() {
        constexpr uint8_t read_Padc_MSB = 0x80;
        constexpr uint8_t read_Padc_LSB = 0x82;
        constexpr uint8_t start_conversions = 0x24;

        uint8_t start[] = {start_conversions, 0x00, 0x00};
        p_spi->spiWrite(p_slave_select, start, sizeof(start));
        delay_ms(10);  // Allow the conversion to finish before reading the registers.

        uint8_t tx_msg[] = {read_Padc_MSB, read_Padc_LSB};
        uint8_t rx_msg[2] = {};
        p_spi->spiAlternate(p_slave_select, tx_msg, rx_msg, sizeof(rx_msg));

        // The 10-bit result uses all of the first byte and the top two bits of the second.
        return (rx_msg[0] << 2) | (rx_msg[1] >> 6);
    }

private:
    spiPtr p_spi;
    int* p_slave_select;
    void (*delay_ms)(uint32_t);
};

// Select the SPI backend here. The IC driver receives a pointer to this bus.
// The Teensy chip-select descriptor contains one GPIO pin number.
int slave_select[] = {2};
constexpr uint32_t spi_clock_hz = 300000;
SPISettings spi0_settings(spi_clock_hz, MSBFIRST, SPI_MODE0);

using SpiBus = TeensySPI;
SpiBus spi0(&SPI);

// Supply the Teensy delay function for the sensor conversion.
void delayMs(uint32_t ms) {
    delay(ms);
}

MPL115A1<SpiBus*> sensor(&spi0, slave_select, delayMs);

// Start SPI and serial output with chip select idle high.
void setup() {
    pinMode(slave_select[0], OUTPUT);
    digitalWrite(slave_select[0], HIGH);
    SPI.begin();
    Serial.begin(115200);
}

// Print one raw pressure reading per second.
void loop() {
    SPI.beginTransaction(spi0_settings);
    const uint16_t P_ADC = sensor.readPadc();
    SPI.endTransaction();

    Serial.println(P_ADC);
    delay(1000);
}
