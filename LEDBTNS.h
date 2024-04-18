
#ifndef PF_LED_BTNS_H
#define PF_LED_BTNS_H

// Includes
#include <stdint.h>

// Button Registers
#define SW1_REG 0x10
#define SW2_REG 0x01

// Prototypes
void LEDBTNS_Init(void);

void FlashTimer_Init(void);

void Timer0A_Handler(void);

void GPIOPortF_Handler(void);

void SetLED(uint8_t data);

void FlashLED(uint8_t data);

#endif
