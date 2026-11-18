/*
 * TeensySPI.cpp
 *
 *  Created on: Jun 7, 2020
 *      Author: Bryan
 */

#include "TeensySPI.h"

#include "Arduino.h"
#include "SPI.h"

//#include <stdint.h>

// Store the SPI bus supplied by the caller.
TeensySPI::TeensySPI(SPIClass* p_spi_bus_){
	p_spi_bus = p_spi_bus_;
}
// Leave the caller-owned SPI bus in place.
TeensySPI::~TeensySPI(){}

// Drive the selected GPIO pin high.
void TeensySPI::pinHigh(int* p_pin){
	digitalWrite(*p_pin, HIGH);
}

// Drive the selected GPIO pin low.
void TeensySPI::pinLow(int* p_pin){
	digitalWrite(*p_pin, LOW);
}

// Transfer 1 byte of data to a 1 byte address
void TeensySPI::spiWriteRegister(int* slave_select, uint8_t address, uint8_t tx_byte){
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(address);
	p_spi_bus->transfer(tx_byte);

	digitalWrite(*slave_select, HIGH);
}

// Transfer 2 bytes of data to a 1 byte address
void TeensySPI::spiWriteRegister(int* slave_select, uint8_t address, uint16_t tx_bytes){
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(address);
	p_spi_bus->transfer16(tx_bytes);  // Uses the bit order set on the SPI bus.

	digitalWrite(*slave_select, HIGH);
}

// Transfer 2 bytes of data to a 2 byte address
void TeensySPI::spiWriteRegister(int* slave_select, uint16_t address, uint16_t tx_bytes){
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer16(address);
	p_spi_bus->transfer16(tx_bytes);  // Uses the bit order set on the SPI bus.

	digitalWrite(*slave_select, HIGH);
}

// Transfer n bytes of data to a 1 byte address
void TeensySPI::spiWriteRegister(int* slave_select, uint8_t address, uint8_t* tx_bytes, int n_tx_bytes){
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(address);
	for(int ii = 0; ii < n_tx_bytes; ii++){
		p_spi_bus->transfer(tx_bytes[ii]);
	}

	digitalWrite(*slave_select, HIGH);
}

// Transfer n bytes of data to a 2 byte address
void TeensySPI::spiWriteRegister(int* slave_select, uint16_t address, uint8_t* tx_bytes, int n_tx_bytes){
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer16(address);
	for(int ii = 0; ii < n_tx_bytes; ii++){
		p_spi_bus->transfer(tx_bytes[ii]);
	}

	digitalWrite(*slave_select, HIGH);
}

// Send an 8-bit register address and clock in one byte.
uint8_t TeensySPI::spiReadRegister(int* slave_select, uint8_t address){
	uint8_t blank_tx = 0x00;  // SPI needs a transmitted byte to clock in a received byte.
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(address);
	uint8_t rx_byte = p_spi_bus->transfer(blank_tx);

	digitalWrite(*slave_select, HIGH);
	return rx_byte;
}

// Send an 8-bit register address and clock in a buffer under the same chip select.
void TeensySPI::spiReadRegister(int* slave_select, uint8_t address, uint8_t* rx_bytes, int n_rx_bytes){
	uint8_t blank_tx = 0x00;  // SPI needs a transmitted byte to clock in a received byte.
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(address);
	for(int ii = 0; ii < n_rx_bytes; ii++){
		rx_bytes[ii] = p_spi_bus->transfer(blank_tx);
	}
	digitalWrite(*slave_select, HIGH);
}

// Send a single byte under one chip-select assertion.
void TeensySPI::spiWrite(int* slave_select, uint8_t tx_byte){
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(tx_byte);

	digitalWrite(*slave_select, HIGH);
}

// Send a buffer under one chip-select assertion.
void TeensySPI::spiWrite(int* slave_select, uint8_t* tx_bytes, int n_bytes){
	digitalWrite(*slave_select, LOW);

	for(int ii = 0; ii < n_bytes; ii++){
		p_spi_bus->transfer(tx_bytes[ii]);
	}
	digitalWrite(*slave_select, HIGH);
}

// Clock in one byte by transmitting a dummy zero.
uint8_t TeensySPI::spiRead(int* slave_select){
	uint8_t blank_tx = 0x00;  // SPI needs a transmitted byte to clock in a received byte.
	digitalWrite(*slave_select, LOW);

	uint8_t rx_byte = p_spi_bus->transfer(blank_tx);

	digitalWrite(*slave_select, HIGH);
	return rx_byte;
}

// Clock in a buffer by transmitting one dummy zero per byte.
void TeensySPI::spiRead(int* slave_select, uint8_t* rx_bytes, int n_bytes){
	uint8_t blank_tx = 0x00;  // SPI needs a transmitted byte to clock in a received byte.
	digitalWrite(*slave_select, LOW);
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = p_spi_bus->transfer(blank_tx);
	}
	digitalWrite(*slave_select, HIGH);
}

// Send and receive one byte in parallel on each transfer.
void TeensySPI::spiExchange(int* slave_select, uint8_t* tx_bytes, uint8_t* rx_bytes, int n_bytes){
	digitalWrite(*slave_select, LOW);
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = p_spi_bus->transfer(tx_bytes[ii]);
	}
	digitalWrite(*slave_select, HIGH);
}

// Alternate each outgoing byte with a separate transfer to receive its response.
void TeensySPI::spiAlternate(int* slave_select, uint8_t* tx_bytes, uint8_t* rx_bytes, int n_bytes){
	uint8_t blank_tx = 0x00;  // SPI needs a transmitted byte to clock in a received byte.
	digitalWrite(*slave_select, LOW);
	for(int ii = 0; ii < n_bytes; ii++){
		p_spi_bus->transfer(tx_bytes[ii]);
		rx_bytes[ii] = p_spi_bus->transfer(blank_tx);
	}
	digitalWrite(*slave_select, HIGH);
}

// Send the command bytes, then clock in the response while chip select stays low.
void TeensySPI::spiWriteThenRead(int* slave_select, uint8_t* tx_bytes, int n_tx_bytes, uint8_t* rx_bytes, int n_rx_bytes){
	uint8_t blank_tx = 0x00;  // SPI needs a transmitted byte to clock in a received byte.
	digitalWrite(*slave_select, LOW);
	// digitalWrite(6, LOW);
	// Serial.print("slave_select_pin: \t"); Serial.println(*slave_select);

	for(int ii = 0; ii < n_tx_bytes; ii++){
		p_spi_bus->transfer(tx_bytes[ii]);
	}

	for(int ii = 0; ii < n_rx_bytes; ii++){
		rx_bytes[ii] = p_spi_bus->transfer(blank_tx);
	}

	digitalWrite(*slave_select, HIGH);
	// digitalWrite(6, HIGH);
}

// void TeensySPI::spiWriteThenExchange(int* slave_select, uint8_t* tx_bytes, int rx_after, uint8_t* rx_bytes, int n_rx_bytes){
// 	digitalWrite(*slave_select, LOW);

// 	for(int ii = 0; ii < rx_after; ii++){
// 		p_spi_bus->transfer(tx_bytes[ii]);
// 	}

// 	for(int ii = 0; ii < n_rx_bytes; ii++){
// 		rx_bytes[ii] = p_spi_bus->transfer(tx_bytes[ii]);
// 	}

// 	digitalWrite(*slave_select, HIGH);
// }
