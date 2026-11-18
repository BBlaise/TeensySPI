/*
 * Teensyp_spi->cpp
 *
 *  Created on: Jun 7, 2020
 *      Author: Bryan
 */

#include "TeensySPI.h"

#include "Arduino.h"
#include "SPI.h"

//#include <stdint.h>

// Constructor
//TeensySPI::TeensySPI(){}
TeensySPI::TeensySPI(SPIClass* p_spi_bus_){
	p_spi_bus = p_spi_bus_;
}
// Deconstructor
TeensySPI::~TeensySPI(){}

void TeensySPI::pinHigh(int* p_pin){
	digitalWrite(*p_pin, HIGH);
}

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
	p_spi_bus->transfer16(tx_bytes);

	digitalWrite(*slave_select, HIGH);
}

// Transfer 2 bytes of data to a 2 byte address
void TeensySPI::spiWriteRegister(int* slave_select, uint16_t address, uint16_t tx_bytes){
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer16(address);
	p_spi_bus->transfer16(tx_bytes);

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

uint8_t TeensySPI::spiReadRegister(int* slave_select, uint8_t address){
	uint8_t blank_tx = 0x00;
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(address);
	uint8_t rx_byte = p_spi_bus->transfer(blank_tx);

	digitalWrite(*slave_select, HIGH);
	return rx_byte;
}

void TeensySPI::spiReadRegister(int* slave_select, uint8_t address, uint8_t* rx_bytes, int n_rx_bytes){
	uint8_t blank_tx = 0x00;
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(address);
	for(int ii = 0; ii < n_rx_bytes; ii++){
		rx_bytes[ii] = p_spi_bus->transfer(blank_tx);
	}
	digitalWrite(*slave_select, HIGH);
}

void TeensySPI::spiWrite(int* slave_select, uint8_t tx_byte){
	digitalWrite(*slave_select, LOW);

	p_spi_bus->transfer(tx_byte);

	digitalWrite(*slave_select, HIGH);
}

void TeensySPI::spiWrite(int* slave_select, uint8_t* tx_bytes, int n_bytes){
	digitalWrite(*slave_select, LOW);

	for(int ii = 0; ii < n_bytes; ii++){
		p_spi_bus->transfer(tx_bytes[ii]);
	}
	digitalWrite(*slave_select, HIGH);
}

uint8_t TeensySPI::spiRead(int* slave_select){
	// Not sure how useful this function is
	uint8_t blank_tx = 0x00;
	digitalWrite(*slave_select, LOW);

	uint8_t rx_byte = p_spi_bus->transfer(blank_tx);

	digitalWrite(*slave_select, HIGH);
	return rx_byte;
}

void TeensySPI::spiRead(int* slave_select, uint8_t* rx_bytes, int n_bytes){
	uint8_t blank_tx = 0x00;
	digitalWrite(*slave_select, LOW);
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = p_spi_bus->transfer(blank_tx);
	}
	digitalWrite(*slave_select, HIGH);
}

void TeensySPI::spiExchange(int* slave_select, uint8_t* tx_bytes, uint8_t* rx_bytes, int n_bytes){
	digitalWrite(*slave_select, LOW);
	for(int ii = 0; ii < n_bytes; ii++){
		rx_bytes[ii] = p_spi_bus->transfer(tx_bytes[ii]);
	}
	digitalWrite(*slave_select, HIGH);
}

void TeensySPI::spiAlternate(int* slave_select, uint8_t* tx_bytes, uint8_t* rx_bytes, int n_bytes){
	uint8_t blank_tx = 0x00;
	digitalWrite(*slave_select, LOW);
	for(int ii = 0; ii < n_bytes; ii++){
		p_spi_bus->transfer(tx_bytes[ii]);
		rx_bytes[ii] = p_spi_bus->transfer(blank_tx);
	}
	digitalWrite(*slave_select, HIGH);
}

void TeensySPI::spiWriteThenRead(int* slave_select, uint8_t* tx_bytes, int n_tx_bytes, uint8_t* rx_bytes, int n_rx_bytes){
	uint8_t blank_tx = 0x00;
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
