/*!*******************************************************************
    @details Header for library to transceive UART communications via Radio
**********************************************************************/
#ifndef RADIOLIB_H
#define RADIOLIB_H

// Library Includes
#include <stdint.h>
#include <math.h>
#include "tm4c123gh6pm.h"

// Prototypes
void Radio_Init(uint32_t baud, uint16_t word, uint16_t stop, uint16_t parity, uint16_t parity_toggle);

void Radio_Tx(uint32_t data);

uint32_t Radio_Rx(void);

void UART1_Handler(void);

#endif



