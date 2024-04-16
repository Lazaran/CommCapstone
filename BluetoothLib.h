#ifndef BLUETOOTHLIB_H
#define BLUETOOTHLIB_H

// Defines
#include <stdint.h>
#include <math.h>
#include "tm4c123gh6pm.h"

extern volatile uint8_t out_char;

void Bluetooth_Init(uint32_t baud, uint16_t word, uint16_t stop, uint16_t parity, uint16_t parity_toggle);

void PortF_Init(void);

void Bluetooth_Tx(uint8_t data);

uint8_t Bluetooth_Rx(void);

void UART5_Handler(void);

#endif



