/*
 * TeensySPI.h
 *
 *  Created on: Jun 7, 2020
 *      Author: Bryan
 */

#ifndef TEENSYSPI_H_
#define TEENSYSPI_H_

#include <stdint.h>

#include "Arduino.h"
#include "SPI.h"

class TeensySPI {
public:
	//TeensySPI();
	TeensySPI(SPIClass* p_spi_bus_);
	~TeensySPI();

	SPIClass* p_spi_bus;

	// Methods
	void pinHigh(int* p_pin);
	void pinLow(int* p_pin);
	void spiWriteRegister(int* slave_select, uint8_t address, uint8_t tx_byte);
	void spiWriteRegister(int* slave_select, uint8_t address, uint16_t tx_bytes);
	void spiWriteRegister(int* slave_select, uint16_t address, uint16_t tx_bytes);
	void spiWriteRegister(int* slave_select, uint8_t address, uint8_t* tx_bytes, int n_tx_bytes);
	void spiWriteRegister(int* slave_select, uint16_t address, uint8_t* tx_bytes, int n_tx_bytes);
	uint8_t spiReadRegister(int* slave_select, uint8_t address);
	void spiReadRegister(int* slave_select, uint8_t address, uint8_t* rx_bytes, int n_rx_bytes);
	void spiWrite(int* slave_select, uint8_t tx_byte);
	void spiWrite(int* slave_select, uint8_t* tx_bytes, int n_bytes);
	uint8_t spiRead(int* slave_select);
	void spiRead(int* slave_select, uint8_t* rx_bytes, int n_bytes);
	void spiExchange(int* slave_select, uint8_t* tx_bytes, uint8_t* rx_bytes, int n_bytes);
	void spiAlternate(int* slave_select, uint8_t* tx_bytes, uint8_t* rx_bytes, int n_bytes);
	void spiWriteThenRead(int* slave_select, uint8_t* tx_bytes, int n_tx_bytes, uint8_t* rx_bytes, int n_rx_bytes);
	void spiWriteThenExchange(int* slave_select, uint8_t* tx_bytes, int rx_after, uint8_t* rx_bytes, int n_rx_bytes);
};

#endif /* TEENSYSPI_H_ */
