/*!*******************************************************************
    @details Header for Library to transceive UART communications via Bluetooth
**********************************************************************/
#ifndef BLUETOOTHLIB_H
#define BLUETOOTHLIB_H

// Library Includes
#include <stdint.h>
#include <math.h>
#include "tm4c123gh6pm.h"

void Bluetooth_Init(uint32_t baud, uint16_t word, uint16_t stop, uint16_t parity, uint16_t parity_toggle);

void Bluetooth_Tx(uint32_t data);

uint32_t Bluetooth_Rx(void);

void UART5_Handler(void);

#endif



