#ifndef RADIOLIB_H
#define RADIOLIB_H

#include <stdint.h>
#include <math.h>
#include "tm4c123gh6pm.h"

void Radio_Init(uint32_t baud, uint16_t word, uint16_t stop, uint16_t parity, uint16_t parity_toggle);

void Radio_Tx(uint8_t data);

uint8_t Radio_Rx(void);

void UART1_Handler(void);

#endif



