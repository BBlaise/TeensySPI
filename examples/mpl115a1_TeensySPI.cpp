// Read an MPL115A1 raw pressure ADC value over SPI and print it once per second.
#include <Arduino.h>
#include <SPI.h>
#include "TeensySPI.h"

namespace {
int slave_select = 2;
TeensySPI spi0(&SPI);
SPISettings spi0_settings(300000, MSBFIRST, SPI_MODE0);

// Start a conversion, then read and combine the two pressure ADC register bytes.
uint16_t readPadc() {
    constexpr uint8_t read_Padc_MSB = 0x80;
    constexpr uint8_t read_Padc_LSB = 0x82;
    constexpr uint8_t start_conversions = 0x24;

    uint8_t start[] = {start_conversions, 0x00, 0x00};
    SPI.beginTransaction(spi0_settings);
    spi0.spiWrite(&slave_select, start, sizeof(start));
    SPI.endTransaction();
    delay(10);  // Allow the conversion to finish before reading the registers.

    uint8_t tx_msg[] = {read_Padc_MSB, read_Padc_LSB};
    uint8_t rx_msg[2] = {};
    SPI.beginTransaction(spi0_settings);
    spi0.spiAlternate(&slave_select, tx_msg, rx_msg, sizeof(rx_msg));
    SPI.endTransaction();

    // The 10-bit result uses all of the first byte and the top two bits of the second.
    return (rx_msg[0] << 2) | (rx_msg[1] >> 6);
}
}

// Start SPI and serial output with chip select idle high.
void setup() {
    pinMode(slave_select, OUTPUT);
    digitalWrite(slave_select, HIGH);
    SPI.begin();
    Serial.begin(115200);
}

// Print one raw pressure reading per second.
void loop() {
    const uint16_t P_ADC = readPadc();
    Serial.println(P_ADC);
    delay(1000);
}
